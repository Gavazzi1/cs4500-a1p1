//lang::CwC
#include <stdlib.h> 
#include <iostream>
#include <stdio.h>
#include <string.h>     /* strcmp */

#include "sorer.h"
#include "column.h"
#include "helpers.h"

void int_row(SOR* reader) {
    int base = 0;
    for (unsigned int j = 0; j < reader->cols_[0]->len_; ++j) {
        for (unsigned int i = 0; i < reader->len_; ++i) {
            char* val = reader->get_value(i, j);
            base = base + atoi(val);
            delete[] val;
        }
    }
}

void int_col(SOR* reader) {
    int base = 0;
    for (int i = 0; i < reader->len_; ++i) {
        for (int j = 0; j < reader->cols_[0]->len_; ++j) {
            char* val = reader->get_value(i, j);
            base = base + atoi(val);
            delete[] val;
        }
    }
}

void float_row(SOR* reader) {
    float base = 0;
    for (int j = 0; j < reader->cols_[0]->len_; ++j) {
        for (int i = 0; i < reader->len_; ++i) {
            char* val = reader->get_value(i, j);
            base = base + atof(val);
            delete[] val;
        }
    }
}

void float_col(SOR* reader) {
    float base = 0;
    for (int i = 0; i < reader->len_; ++i) {
        for (int j = 0; j < reader->cols_[0]->len_; ++j) {
            char* val = reader->get_value(i, j);
            base = base + atof(val);
            delete[] val;
        }
    }
}

void bool_col(SOR* reader) {
    bool base = false;
    for (int i = 0; i < reader->len_; ++i) {
        for (int j = 0; j < reader->cols_[0]->len_; ++j) {
            char* val = reader->get_value(i, j);
            base = base ^ (strcmp(val, "0") == 0);
            delete[] val;
        }
    }
}

void bool_row(SOR* reader) {
    bool base = false;
    for (int j = 0; j < reader->cols_[0]->len_; ++j) {
        for (int i = 0; i < reader->len_; ++i) {
            char* val = reader->get_value(i, j);
            base = base ^ (strcmp(val, "0") == 0);
            delete[] val;
        }
    }
}

void string_col(SOR* reader) {
    int base = 0;
    for (int i = 0; i < reader->len_; ++i) {
        for (int j = 0; j < reader->cols_[0]->len_; ++j) {
            char* val = reader->get_value(i, j);
            base = base + strlen(val);
            delete[] val;
        }
    }
}

void string_row(SOR* reader) {
    int base = 0;
    for (int j = 0; j < reader->cols_[0]->len_; ++j) {
        for (int i = 0; i < reader->len_; ++i) {
            char* val = reader->get_value(i, j);
            base = base + strlen(val);
            delete[] val;
        }
    }
}

void benchmark(char* dType, char* pattern, SOR* reader) {
    if (strcmp(dType, "int") == 0)
        if (strcmp(pattern, "byrow") == 0)
            int_row(reader);
        else if (strcmp(pattern, "bycol") == 0)
            int_col(reader);
    else if (strcmp(dType, "float") == 0)
        if (strcmp(pattern, "byrow") == 0)
            float_row(reader);
        else if (strcmp(pattern, "bycol") == 0)
            float_col(reader);
    else if (strcmp(dType, "bool") == 0)
        if (strcmp(pattern, "byrow") == 0)
            bool_row(reader);
        else if (strcmp(pattern, "bycol") == 0)
            bool_col(reader);
    else if (strcmp(dType, "string") == 0)
        if (strcmp(pattern, "byrow") == 0)
            string_row(reader);
        else if (strcmp(pattern, "bycol") == 0)
            string_col(reader);     
}

int main(int argc, char** argv) {

    // have these flags appeared
    bool a_file, a_from, a_len;
    a_file = a_from = a_len = false;
    int a_type, a_print, a_missing;
    a_type = a_print = a_missing = 0;
    char *file_name;
    unsigned int from, len;
    unsigned int col_arg1, col_arg2;

    char* dType;
    char* pattern;
    bool dobenchmark = false;

    // if user passes in non integers for places where there should be integers
    // just default the value to 0
    int index = 1; // skipping the first argument
    while (index < argc) {
        if (strcmp(argv[index], "-f") == 0) {
            check_in_bounds(argc, index, 1);
            file_name = argv[index + 1];
            a_file = true;
            index+=2;
        }
        else if (strcmp(argv[index], "-from") == 0) {
            check_in_bounds(argc, index, 1);
            from = check_positive(atoi(argv[index + 1]));
            a_from = true;
            index+=2;
        }
        else if (strcmp(argv[index], "-len") == 0) {
            check_in_bounds(argc, index, 1);
            len = check_positive(atoi(argv[index + 1]));
            a_len = true;
            index+=2;
        }
        else if (strcmp(argv[index], "-print_col_type") == 0) {
            check_in_bounds(argc, index, 1);
            col_arg1 = check_positive(atoi(argv[index + 1]));
            a_type = 1;
            index+=2;
        }
        else if (strcmp(argv[index], "-print_col_idx") == 0) {
            check_in_bounds(argc, index, 2);
            col_arg1 = check_positive(atoi(argv[index + 1]));
            col_arg2 = check_positive(atoi(argv[index + 2]));
            a_print = 1;
            index+=3;
        }
        else if (strcmp(argv[index], "-is_missing_idx") == 0) {
            check_in_bounds(argc, index, 2);
            col_arg1 = check_positive(atoi(argv[index + 1]));
            col_arg2 = check_positive(atoi(argv[index + 2]));
            a_missing = 1;
            index+=3;
        }
        else if (strcmp(argv[index], "-type") == 0) {
            dType = argv[index + 1];
            index += 2;
            dobenchmark = true;
        }
        else if (strcmp(argv[index], "-pattern") == 0) {
            pattern = argv[index + 1];
            index += 2;
            dobenchmark = true;
        }
        else {
            affirm(false, "Invalid input argument");
        }
    }

    affirm(a_file, "Missing file");
    affirm(a_from, "Missing from");
    affirm(a_len, "Missing length");

    // opening the file
    FILE *f = fopen(file_name, "r");
    affirm(f != NULL, "File is null pointer");

    SOR* reader = new SOR();
    reader->read(f, from, len);

    if (dobenchmark) {
        benchmark(dType, pattern, reader);
        return 0;
    }

    if (a_type) {
        switch (reader->get_col_type(col_arg1)) {
            case type_string:
                printf("STRING\n");
                break;
            case type_float:
                printf("FLOAT\n");
                break;
            case type_int:
                printf("INT\n");
                break;
            case type_bool:
                printf("BOOL\n");
                break;
            default:
                printf("Index out of bounds.\n");
                break;
        }

    } else if (a_print) {
        char* temp = reader->get_value(col_arg1, col_arg2);
        if (temp != nullptr) {
            printf("%s\n", temp);
        } 
    } else if (a_missing) {
        printf("%d\n", reader->is_missing(col_arg1, col_arg2));
    }

    delete reader;
    fclose(f);
    return 0;
}
