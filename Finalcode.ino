

import RPi.GPIO as GPIO
import tkinter as tk
from tkinter import ttk
import time

# Create a custom Morse code dictionary for letter and number mappings
custom_code_dict = {
    'A': '.-', 'B': '-...', 'C': '-.-.', 'D': '-..', 'E': '.',
    'F': '..-.', 'G': '--.', 'H': '....', 'I': '..', 'J': '.---',
    'K': '-.-', 'L': '.-..', 'M': '--', 'N': '-.', 'O': '---',
    'P': '.--.', 'Q': '--.-', 'R': '.-.', 'S': '...', 'T': '-',
    'U': '..-', 'V': '...-', 'W': '.--', 'X': '-..-', 'Y': '-.--',
    'Z': '--..', '0': '-----', '1': '.----', '2': '..---', '3': '...--',
    '4': '....-', '5': '.....', '6': '-....', '7': '--...', '8': '---..',
    '9': '----.', ' ': '/'  # Representing space with '/'
}

# Set up GPIO: Configure the LED pin for output
custom_LED_PIN = 7  # Adjust this to match the actual GPIO pin
GPIO.setmode(GPIO.BOARD)
GPIO.setup(custom_LED_PIN, GPIO.OUT)

# Function to transmit custom Morse code using LED blinking
def custom_blink_morse_code(text):
    for char in text.upper():
        if char in custom_code_dict:
            morse_code = custom_code_dict[char]
            for symbol in morse_code:
                if symbol == '.':
                    GPIO.output(custom_LED_PIN, GPIO.HIGH)
                    time.sleep(0.1)  # Duration of a dot
                    GPIO.output(custom_LED_PIN, GPIO.LOW)
                    time.sleep(0.1)  # Gap between dots and dashes
                elif symbol == '-':
                    GPIO.output(custom_LED_PIN, GPIO.HIGH)
                    time.sleep(0.3)  # Duration of a dash
                    GPIO.output(custom_LED_PIN, GPIO.LOW)
                    time.sleep(0.1)  # Gap between dots and dashes
            time.sleep(0.2)  # Gap between characters
        else:
            # Handle spaces and unknown characters with a longer gap
            time.sleep(0.4)  # Gap between words

root = tk.Tk()
root.title("Custom Text to Morse Code LED Blinker")

# Function to start LED blinking when the button is clicked
def start_custom_blinking():
    text = custom_entry.get()
    custom_blink_morse_code(text)

# Create a frame for the GUI with colorful background
custom_frame = ttk.Frame(root, padding=(20, 20))
custom_frame.grid(column=0, row=0, sticky=(tk.W, tk.E, tk.N, tk.S))
custom_frame.columnconfigure(0, weight=1)
custom_frame.rowconfigure(0, weight=1)

# Create a label with colorful font for instruction
custom_label = ttk.Label(custom_frame, text="Insert a name", font=("Times New Roman", 20), foreground="blue")
custom_label.grid(column=0, row=0, columnspan=2, pady=(0, 20))

# Create an entry widget with colorful background for text input
custom_entry = ttk.Entry(custom_frame, width=20, font=("Arial", 20), background="lightgreen")
custom_entry.grid(column=0, row=1, columnspan=2, pady=(0, 10))

# Create a button with colorful background for starting LED blinking
custom_button = ttk.Button(custom_frame, text="Blink", command=start_custom_blinking)
custom_button.grid(column=0, row=2, columnspan=2, pady=(0, 20))

root.mainloop()2

GPIO.cleanup()