#include <stdio.h>
#include <errno.h>
#include "gridfile.h"
#include "datagenerator.h"

#define SIZE 1000
#define PSIZE 4096
#define NAME "db"
#define NRECORDS 8000000
#define FRECORD 666

int main()
{
	int error = 0;
	int iter = 0;
	int64_t x = 0;
	int64_t y = 0;
	int64_t rsize = 0;
	void *record = NULL;
	struct gridconfig vconfig;
	struct gridfile vgrid;
	int64_t fx = 0;
	int64_t fy = 0;

	vconfig.size = SIZE;
	vconfig.psize = PSIZE;
	vconfig.name = NAME;

	error = vgrid.createGrid(&vconfig);
	if (error < 0) {
		goto clean;
	}

	error = vgrid.loadGrid();
	if (error < 0) {
		goto clean;
	}

	for (iter = 0; iter < NRECORDS; iter++) {
		getRandomRecord(&x, &y, &rsize, &record);

		if (iter == FRECORD) {
			fx = x;
			fy = y;
		}

		error = vgrid.insertRecord(x, y, record, rsize);
		if (error < 0) {
			goto pclean;
		}

		free(record);
	}

	error = vgrid.findRecord(fx, fy, &record);
	if (error < 0) {
		goto clean;
	}

	error = vgrid.deleteRecord(fx, fy);
	if (error < 0) {
		goto clean;
	}

	error = vgrid.findRecord(fx, fy, &record);
	if (error == -EINVAL) {
		error = 0;
	} else if (error == 0) {
		error = -EINVAL;
	}

 pclean:
	vgrid.unloadGrid();

 clean:
	printf("Error: %d\n", error);
	return error;
}
