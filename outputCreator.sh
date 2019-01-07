#!/bin/sh
make cleanAll
make
./recommender -w Input_Files/1-wax_data_text.txt
./recommender -t Input_Files/2a-test_data_very_large.txt
./recommender -s Input_Files/3a-snow_in_1.txt 100 > Output_Files/1-2a-3a.out
./recommender -s Input_Files/3b-snow_in_2.txt 100 > Output_Files/1-2a-3b.out
./recommender -s Input_Files/3c-snow_in_3.txt 100 > Output_Files/1-2a-3c.out
./recommender -s Input_Files/3d-snow_in_4.txt 100 > Output_Files/1-2a-3d.out
./recommender -s Input_Files/3e-snow_in_5.txt 100 > Output_Files/1-2a-3e.out
make cleanAll
make
./recommender -w Input_Files/1-wax_data_text.txt
./recommender -t Input_Files/2b-test_data_large.txt
./recommender -s Input_Files/3a-snow_in_1.txt 100 > Output_Files/1-2b-3a.out
./recommender -s Input_Files/3b-snow_in_2.txt 100 > Output_Files/1-2b-3b.out
./recommender -s Input_Files/3c-snow_in_3.txt 100 > Output_Files/1-2b-3c.out
./recommender -s Input_Files/3d-snow_in_4.txt 100 > Output_Files/1-2b-3d.out
./recommender -s Input_Files/3e-snow_in_5.txt 100 > Output_Files/1-2b-3e.out
make cleanAll
make
./recommender -w Input_Files/1-wax_data_text.txt
./recommender -t Input_Files/2c-test_data_medium.txt
./recommender -s Input_Files/3a-snow_in_1.txt 100 > Output_Files/1-2c-3a.out
./recommender -s Input_Files/3b-snow_in_2.txt 100 > Output_Files/1-2c-3b.out
./recommender -s Input_Files/3c-snow_in_3.txt 100 > Output_Files/1-2c-3c.out
./recommender -s Input_Files/3d-snow_in_4.txt 100 > Output_Files/1-2c-3d.out
./recommender -s Input_Files/3e-snow_in_5.txt 100 > Output_Files/1-2c-3e.out
make cleanAll
make
./recommender -w Input_Files/1-wax_data_text.txt
./recommender -t Input_Files/2d-test_data_small.txt
./recommender -s Input_Files/3a-snow_in_1.txt 100 > Output_Files/1-2d-3a.out
./recommender -s Input_Files/3b-snow_in_2.txt 100 > Output_Files/1-2d-3b.out
./recommender -s Input_Files/3c-snow_in_3.txt 100 > Output_Files/1-2d-3c.out
./recommender -s Input_Files/3d-snow_in_4.txt 100 > Output_Files/1-2d-3d.out
./recommender -s Input_Files/3e-snow_in_5.txt 100 > Output_Files/1-2d-3e.out
