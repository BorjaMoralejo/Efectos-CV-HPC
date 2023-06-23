import csv
import sys

def extract_columns(filename, columns, output_filename):
    with open(filename, 'r') as file:
        reader = csv.reader(file,delimiter=':')
        extracted_data = []

        for row in reader:
            extracted_row = [row[int(column)] for column in columns]
            extracted_data.append(extracted_row)

    with open(output_filename, 'w', newline='') as output_file:
        writer = csv.writer(output_file,delimiter=':')
        writer.writerows(extracted_data)

    print("Columnas extraídas y guardadas en", output_filename)

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Uso: python extract_columns.py archivo.csv columnas output.csv")
        sys.exit(1)

    filename = sys.argv[1]
    columns = sys.argv[2].split(',')
    output_filename = sys.argv[3]

    extract_columns(filename, columns, output_filename)
