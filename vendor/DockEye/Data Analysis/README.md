# Data analysis 
This folder houses all data files associated with the testing of the DockEye. 

## Raw data
The raw data folder has all data that is written to the DockEye's SD card. This will need to be cleaned, as in its current state it is not very clear what measurements are associated with what. There are problems with the RTC (as noted in the issues tab) but this is not currently a priority so the data files are not very easy to navigate. The compiled data spreadsheet can be used as a temporary guide to navigate these files.

## Compiled data
The compiled data file has all of the averaged measurements from each run of the DockEye, alongside the respective Horiba Fluoromax-4 measurements, and also has calibrated measurements from the calibration curve of each.

## Scripts
The scripts folder has Python and MATLAB scripts for use in data analysis. These scripts expect the data files in 3 columns: Date, DockEye calibrated chlorophyll-a, and Horiba calibrated chlorophyll-a. Up until this point (Feburary 2026), MATLAB has been used as the primary data analysis language. However, due to the open-source of this project, going forward, Python will become the main language. MATLAB scripts will continue to be made available for anyone who prefers that, but updates are not planned. Feel free to use and update these.
