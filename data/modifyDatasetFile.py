import csv

# File paths
input_file = './soc-sign-bitcoinalpha-mod.csv'
output_file = './data.csv'


# CODE TO REMOVE THE NEGATIVE VALUES IN THE THIRD COLUMN :

# # Read and process the CSV
with open(input_file, mode='r', newline='') as infile:
    reader = csv.reader(infile)
    rows = []

    for i, row in enumerate(reader):
        if i == 0:
            # Header row, keep as is
            rows.append(row)
        else:
            # Apply abs() to third column and convert to integer
            try:
                row[2] = str(abs(int(float(row[2]))))
            except ValueError:
                pass  # Ignore if conversion fails
            rows.append(row)

# Write to new CSV
with open(output_file, mode='w', newline='') as outfile:
    writer = csv.writer(outfile)
    writer.writerows(rows)

print(f"Processed data saved to {output_file}")




# CODE TO CHECK IF THERE NO NEGATIVE VALUES IN THE THIRD COLUMN :

def check_negative_values_in_third_column(file_path):
    """
    Checks if there are any negative values in the third column of the input CSV file.

    :param file_path: Path to the input CSV file
    :return: True if negative values are found, False otherwise
    """
    with open(file_path, mode='r', newline='') as infile:
        reader = csv.reader(infile)
        for i, row in enumerate(reader):
            if i == 0:
                # Skip the header row
                continue
            try:
                # Check if the third column value is negative
                if float(row[2]) < 0:
                    return True
            except (ValueError, IndexError):
                # Ignore rows with invalid or missing data
                pass
    return False

# Main execution
if __name__ == "__main__":
    if check_negative_values_in_third_column(output_file):
        print("The input file contains negative values in the third column.")
    else:
        print("The input file contains only positive values in the third column.")