# Velocity Curve Generator

This Python script generates a velocity curve lookup table for use in the keyboard scanner firmware.

## What it does

The script creates a cubic velocity curve that maps linear input values (0-127) to exponentially scaled output values (0-127).

## How it works

1. **Generates a curve**: Uses some function to create the desired response curve
2. **Creates lookup table**: Converts the curve into 128 discrete integer values (0-127)
3. **Outputs C header file**: Generates `../velocity.h` with `VELOCITY_CURVE[]` array containing the lookup values
4. **Visualizes the curve**: Displays a plot showing the input vs output relationship

## Requirements

- Python 3.x
- Dependencies listed in `requirements.txt`

Install dependencies:
```bash
pip install -r requirements.txt
```

## Usage

Simply run the script:
```bash
python velocity.py
```

This will:
1. Generate the `../velocity.h` header file
2. Display a plot of the velocity curve
