import pandas as pd
import numpy as np
from scipy.stats import norm
import math
import os
import sys

# Function to compute weighted mean and standard deviation of a list of values
def get_mean_and_std(values, weights):
    average = np.average(values, weights=weights)
    variance = np.average((values-average)**2, weights=weights)
    return (average, math.sqrt(variance))

# Function to compute the confidence interval for a population mean, using a normal distribution
def get_ci(alpha, std, size):
    return std * norm.ppf(1 - alpha/2) / np.sqrt(size)

# Check that the filename prefix was passed as an argument
if len(sys.argv) != 2:
    print("Usage: python script.py <filename_prefix>")
    sys.exit(1)

# Get the filename prefix from the command-line argument
filename_prefix = sys.argv[1]

# Initialize lists to hold the computed means, standard deviations, confidence intervals, and filenames
mean_list = []
std_list = []
ci_list = []
filename_list = []

# Loop through all the files in the current directory
for filename in os.listdir('.'):
    # Check if the filename matches the given prefix and ends with .csv
    if filename.startswith(filename_prefix) and filename.endswith('.csv'):
        print(f'Processing: {filename}')

        # Read the file into a pandas DataFrame
        df = pd.read_csv(filename, sep=';')

        # Loop through every other column of the DataFrame
        for i in range (0, len(df.columns), 2):
            # Extract the column containing the number of jobs, and drop any rows with missing values
            x = pd.DataFrame(df.iloc[:, i])
            x = x.dropna()
            time = x.values

            time_period = time[-1] - time[0]
            weights = []
            # Compute the weight calculating the duration for which that value holds
            for j in range(len(time) - 1):
                weights.append((time[j+1] - time[j]) / time_period)

            weights = np.asarray(weights)

            x = pd.DataFrame(df.iloc[:, i+1])
            x = x.dropna()
            jobs = x.values[:-1]
            # Compute the mean, standard deviation, and confidence interval of the number of jobs
            [mean, std] = get_mean_and_std(jobs, weights)
            ci = get_ci(0.05, std, len(jobs))
            mean_list.append(mean)
            std_list.append(std)
            ci_list.append(ci)
            filename_list.append(filename)

# Create a Pandas dataframe
df = pd.DataFrame()
df['filename'] = filename_list
df['Mean'] = mean_list
df['std'] = std_list
df['CI'] = ci_list

# Create the output filename by adding "_out" and the suffix to the filename prefix
output_filename = filename_prefix + '_out.csv'

# Write the computed means, standard deviations, and confidence intervals to a new CSV file
df.to_csv(output_filename, sep=',', index=False)

print(f"\nOutput written to -> {output_filename}")