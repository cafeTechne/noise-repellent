/*
noise-repellent -- Noise Reduction LV2

Copyright 2016 Luciano Dato <lucianodato@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/
*/

#include <float.h>
#include <math.h>

//Power subtraction
void power_subtraction(int fft_size_2,
		       float* spectrum,
		       float* noise_thresholds,
		       float* Gk) {

	int k;

	for (k = 0; k <= fft_size_2 ; k++) {
		if (noise_thresholds[k] > FLT_MIN){
			if(spectrum[k] > noise_thresholds[k]){
				Gk[k] = (spectrum[k]-noise_thresholds[k]) / spectrum[k];
			} else {
				Gk[k] = 0.f;
			}
		} else {
			//Otherwise we keep everything as is
			Gk[k] = 1.f;
		}
	}
}

//Gating with envelope smoothing
void spectral_gating(int fft_size_2,
	    float* spectrum,
	    float* noise_thresholds,
	    float* Gk) {

	int k;

	for (k = 0; k <= fft_size_2 ; k++) {
		if (noise_thresholds[k] > FLT_MIN){
			//Hard knee
			if (spectrum[k] >= noise_thresholds[k]){
				//over the threshold
				Gk[k] = 1.f;
			}else{
				//under the threshold
				Gk[k] = 0.f;
			}
		} else {
			//Otherwise we keep everything as is
			Gk[k] = 1.f;
		}
	}
}

//This probably could be better (Postfilter) TODO
void wideband_gating(int fft_size_2,
	    float* spectrum,
	    float* noise_thresholds,
	    float* Gk) {

		int k;
		float x_value = 0.f, n_value = 0.f;

		for (k = 0; k <= fft_size_2 ; k++) {
			x_value +=  spectrum[k];
			n_value += noise_thresholds[k];
		}

		if (n_value > FLT_MIN){

			//Hard knee
			if (x_value >= n_value){
				//over the threshold
				*Gk = 1.f;
			}else{
				//under the threshold
				*Gk = 0.f;
			}
		} else {
			//Otherwise we keep everything as is
			*Gk = 1.f;
	}

}
