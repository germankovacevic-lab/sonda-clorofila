import pandas as pd
import matplotlib.pyplot as plt


# Read the CSV file
df = pd.read_csv('/Users/adam/Documents/chl a data/field test.csv')

# Clean the 'F8 (Raw)' column
df['F8 (Raw)'] = pd.to_numeric(df['F8 (Raw)'], errors='coerce')

# Remove rows with NaN values in the 'F8 (Raw)' column
chlorophyll_data = df.dropna(subset=['F8 (Raw)'])

# Extract 'F8 (Raw)' and 'Milliseconds' values
F8_values = chlorophyll_data['F8 (Raw)']
milliseconds_values = chlorophyll_data['Milliseconds']

# Solve for x using the equation y = 7.6993x + 17.742)
x_values_csv = (F8_values - 17.742) / 0.76993
print(x_values_csv)

# Plot
plt.savefig('chla measurements.png')
plt.plot(milliseconds_values, x_values_csv, marker='o', linestyle='-')
plt.xlabel('Milliseconds')
plt.ylabel('ug/L')
plt.title('Chlorophyll a Measurements vs. Milliseconds')
plt.grid(True)
plt.show()