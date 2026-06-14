// gemachte fehler: 
// mergesort() auf left und right zurueck ueberwiesen anstelle von neuem pointer
// off by one in vergleichen <= anstelle von <
// if size = 1 return items anstelle von neuem pointer -> zeigte auf geloeschten speicher
// i%half_size anstelle von i-half_size -> arithmetic error bei 0%0


double* mergesort(double items[], int size){
    if(size<=1){
        double* result = new double[1];
        result[0] = items[0];
        return result;
    }
    int half_size = size/2;
    double* left = new double[half_size];
    double* right = new double[size-half_size];
    for(int i =0; i<size; i++){
        if(i<half_size) left[i] = items[i];
        else right[i-half_size] = items[i];
    }
    double* sorted_left = mergesort(left, half_size);
    double* sorted_right = mergesort(right, size-half_size);
    delete[] left;
    delete[] right;

    double* result = new double[size];
    int lidx=0,ridx=0;
    for(int idx = 0; idx < size; idx++){
        if(lidx<half_size && ridx<size-half_size){
            if(sorted_left[lidx]<sorted_right[ridx]){
                result[idx] = sorted_left[lidx];
                lidx++;
            } else {
                result[idx] = sorted_right[ridx];
                ridx++;
            }
        } else if (lidx<half_size){
            result[idx] = sorted_left[lidx];
            lidx++;
        } else if (ridx<size-half_size){
            result[idx] = sorted_right[ridx];
            ridx++;
        }
    }
    delete[] sorted_left;
    delete[] sorted_right;
    return result;
}
/* cleanerer merge ist:

double* result = new double[size];
int lidx = 0, ridx = 0;
for (int idx = 0; idx < size; idx++) {
    bool left_done  = lidx >= half_size;
    bool right_done = ridx >= size - half_size;

    if (!left_done && (right_done || sorted_left[lidx] <= sorted_right[ridx])) {
        result[idx] = sorted_left[lidx++];
    } else {
        result[idx] = sorted_right[ridx++];
    }
}

*/