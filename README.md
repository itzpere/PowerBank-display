# Display for powerbank
Parts used:
- 160x80 TFT IPS display
- Gravity switch
- Arduino pro mini V3.3 8Mhz
- 3.3v regulator

## What does this do?
This is designed for powerbank mainly for displaying info in fun and advanced way. It will display battery percetage as a smile so if battery is full it will display a happy face and if not it will be sad (it's dynamic).
For smile it is using simple Bezier curve
reading voltage of battery is done by resistor divider of 22000Kohms and 3300ohms at pin A0.
finaly turning display on is done by gravity switch which will detect if you are shaking the power bank and based on reading turn display on, if you are shaking it even after it is turned on it will go into advanced mode which shows actual % and voltage in numbers.

## What is left?
Some things are still left unfinished because of my carelessness here is a quick list
- Advance reading of every battery cell
- Animations for smile and making it more lively (Moving up and down, being thicker, using colors)
- Detection of charging and discharging 
- Making use of sleep and making code faster/smoother 