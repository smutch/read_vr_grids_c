#include "read_grids.h"

#include <assert.h>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN 256

struct grid read_grid(const char* grid_dir, const int snapshot)
{
    struct grid grid;

    // read in the number of x values and offsets from every grid file
    // nx : number of x-dim values
    // ix_start : first x index
    // n_cell : number of values in each dim
    int* file_nx = NULL;
    int* file_ix_start = NULL;
    int n_cell[3] = { 0, 0, 0 };
    double box_size;
    int n_files = 999;
    const char fname_base[STRLEN] = { "%s/snapshot_%03d.den.%d" };
    {
        for (int ii = 0; ii < n_files; ii++) {
            char fname[STRLEN];
            snprintf(fname, STRLEN, fname_base, grid_dir, snapshot, ii);
            hid_t file_id = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);

            if (ii == 0) {
                herr_t status = H5LTget_attribute_int(file_id, "/", "Num_files", &n_files);
                assert(status >= 0);

                file_nx = calloc(n_files, sizeof(int));
                file_ix_start = calloc(n_files, sizeof(int));

                status = H5LTget_attribute_double(file_id, "/", "BoxSize",
                    &box_size);
                assert(status >= 0);

                status = H5LTget_attribute_int(file_id, "/", "Ngrid_X", n_cell);
                assert(status >= 0);
                status = H5LTget_attribute_int(file_id, "/", "Ngrid_Y", n_cell + 1);
                assert(status >= 0);
                status = H5LTget_attribute_int(file_id, "/", "Ngrid_Z", n_cell + 2);
                assert(status >= 0);
            }

            herr_t status = H5LTget_attribute_int(file_id, "/", "Local_x_start",
                file_ix_start + ii);
            assert(status >= 0);
            status = H5LTget_attribute_int(file_id, "/", "Local_nx", file_nx + ii);
            assert(status >= 0);

            H5Fclose(file_id);
        }
    }

    printf("Reading VELOCIraptor grid for snapshot %d\n", snapshot);

    int total_n_cell = n_cell[0] * n_cell[1] * n_cell[2];
    grid.values = calloc(total_n_cell, sizeof(double));

    for (int ii = 0; ii < n_files; ii++) {
        char fname[STRLEN];
        snprintf(fname, STRLEN, fname_base, grid_dir, snapshot, ii);

        hid_t file_id = H5Fopen(fname, H5F_ACC_RDONLY, H5P_DEFAULT);
        H5LTread_dataset_double(
            file_id, "Density",
            grid.values + (file_ix_start[ii] * n_cell[1] * n_cell[2]));

        H5Fclose(file_id);
    }

    free(file_ix_start);
    free(file_nx);

    memcpy(grid.n_cell, n_cell, sizeof(grid.n_cell));
    grid.box_size = box_size;
    grid.snapshot = snapshot;

    return grid;
}

void free_grid(struct grid* grid) { free(grid->values); }
