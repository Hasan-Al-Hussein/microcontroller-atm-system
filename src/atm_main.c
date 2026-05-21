#include <MKL25Z4.h>
#include <string.h>
#include <stdlib.h>

#define PIN_LENGTH 5
#define TIMEOUT_COUNT 32 // Timeout for keypad scanning (~5 seconds)

char correct_pin[PIN_LENGTH + 1] = "63390"; // Predefined correct PIN

// Function Prototypes
void UART_init(void);
void GPIO_init(void);
void delay_ms(int n);
void sendString(char *str);
void sendChar(char ch);
char receiveChar_withTimeout(int *timeoutFlag);
char keypad_getkey(void);

int main() {
    UART_init(); // Initialize UART communication
    GPIO_init(); // Initialize GPIO for keypad and switch

    int attempts = 0; // Number of PIN attempts
    int timeoutFlag = 0; // Timeout flag for UART input

    while (attempts < 3) {
        sendString("\r\nPress the button to enter PIN...");

        // Wait for button press
        while (!(GPIOC_PDIR & (1UL << 12))) {}
        delay_ms(20); // Debounce
        // Wait for button release
        while (GPIOC_PDIR & (1UL << 12)) {}
        delay_ms(20); // Debounce

        sendString("\r\nEnter 5-digit Pin number: ");
        char entered_pin[PIN_LENGTH + 1]; // Buffer to store entered PIN

        // Read 5 digits from keypad
        for (int i = 0; i < PIN_LENGTH; i++) {
            entered_pin[i] = keypad_getkey();
            if (entered_pin[i] == 0) { // Timeout during PIN entry
                sendString("\r\nTimeout!!\r\n");
                while (1); // Halt the system
            }
            sendChar(entered_pin[i]); // Echo key pressed
        }
        entered_pin[PIN_LENGTH] = '\0'; // Null-terminate the entered PIN

        // Compare entered PIN with correct PIN
        if (strcmp(entered_pin, correct_pin) == 0) {
            sendString("\r\nEntered pin is correct\r\n");
            sendString("Enter amount to be withdrawn: ");

            char amount_str[5]; // Buffer to store withdrawal amount
            // Read 4 digits of amount
            for (int i = 0; i < 4; i++) {
                amount_str[i] = receiveChar_withTimeout(&timeoutFlag);
                if (timeoutFlag) { // Timeout during amount entry
                    sendString("\r\nTimeout!!\r\n");
                    while (1);
                }
                if (amount_str[i] < '0' || amount_str[i] > '9') { // Check if input is digit
                    sendString("\r\nInvalid input! Enter digits only.\r\n");
                    while (1);
                }
                sendChar(amount_str[i]); // Echo entered digit
            }
            amount_str[4] = '\0'; // Null-terminate amount string

            int amount = atoi(amount_str); // Convert ASCII to integer
            // Check balance
            if (amount > 5000) {
                sendString("\r\nInsufficient balance\r\n");
            } else {
                sendString("\r\nTransaction success\r\n");
            }
            while (1); // End program after transaction
        } else {
            attempts++; // Increment wrong attempts
            if (attempts == 3) {
                sendString("\r\nIncorrect Pin. Aborted!\r\n");
                while (1);
            } else {
                sendString("\r\nIncorrect Pin. Try again.\r\n");
            }
        }
    }
}

// UART Initialization
void UART_init(void) {
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;   // Enable UART0 clock
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);   // Select MCGFLLCLK as UART clock source
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;    // Enable PORTA clock

    PORTA_PCR1 = PORT_PCR_MUX(2);         // PTA1 as UART0_TX
    PORTA_PCR2 = PORT_PCR_MUX(2);         // PTA2 as UART0_RX

    UART0_C1 = 0x00;                      // 8-bit data, no parity, 1 stop bit
    UART0_C4 = 0x0F;                      // Over sampling ratio 15
    UART0_BDH = 0x00;                     // Baud rate high byte
    UART0_BDL = 0x89;                     // Baud rate low byte (9600 baud)
    UART0_C2 |= UART0_C2_TE_MASK | UART0_C2_RE_MASK; // Enable UART transmitter and receiver
}

// GPIO Initialization
void GPIO_init(void) {
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK; // Enable clocks for Ports B, E, C

    PORTC_PCR12 = PORT_PCR_MUX(1);        // Configure PTC12 as GPIO (switch)
    GPIOC_PDDR &= ~(1UL << 12);           // Set PTC12 as input

    for (int i = 0; i < 4; i++) {
        PORTB_PCR(i) = PORT_PCR_MUX(1);   // Configure PTB0-PTB3 as GPIO (Rows)
        PORTE_PCR(i) = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // Configure PTE0-PTE3 as GPIO input with pull-up (Columns)
    }
    GPIOB_PDDR |= 0x0F;  // Set PTB0-PTB3 as outputs (rows)
    GPIOE_PDDR &= ~(0x0F); // Set PTE0-PTE3 as inputs (columns)
}

// Keypad Scan
char keypad_getkey(void) {
    const char keymap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    int count = 0;
    while (count < TIMEOUT_COUNT) {
        GPIOB_PSOR = 0x0F; // Set all rows HIGH

        for (int row = 0; row < 4; row++) {
            GPIOB_PCOR = (1 << row); // Drive one row LOW
            delay_ms(20); // Debounce

            int col_val = GPIOE_PDIR & 0x0F; // Read column inputs
            if (col_val != 0x0F) { // Check if any column pulled LOW
                for (int col = 0; col < 4; col++) {
                    if (!(col_val & (1 << col))) {
                        GPIOB_PSOR = 0x0F; // Reset all rows HIGH
                        return keymap[row][col]; // Return detected key
                    }
                }
            }
            GPIOB_PSOR = (1 << row); // Reset current row HIGH
        }
        count++;
    }
    return 0; // Timeout if no key pressed
}

// Delay Function (~1ms per iteration)
void delay_ms(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 16000; j++) {}
    }
}

// Send String through UART
void sendString(char *str) {
    while (*str) {
        while (!(UART0_S1 & UART0_S1_TDRE_MASK)) {} // Wait for transmit buffer empty
        UART0_D = *str++;
    }
}

// Send Single Character through UART
void sendChar(char ch) {
    while (!(UART0_S1 & UART0_S1_TDRE_MASK)) {} // Wait for transmit buffer empty
    UART0_D = ch;
}

// Receive Single Character through UART with Timeout
char receiveChar_withTimeout(int *timeoutFlag) {
    int count = 0;
    while (!(UART0_S1 & UART0_S1_RDRF_MASK)) { // Wait for received data
        delay_ms(1);
        count++;
        if (count > 5000) { // Timeout condition (~5 sec)
            *timeoutFlag = 1;
            return 0;
        }
    }
    *timeoutFlag = 0;
    return UART0_D; // Return received character
}

