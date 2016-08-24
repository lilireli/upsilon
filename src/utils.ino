/**
* @Author: Aurelie Suzanne <aurelie>
* @Date:   21-Mar-2016
* @Project: Upsilon
* @Last modified by:   aurelie
* @Last modified time: 24-Aug-2016
*
* This file contains tools for the project
*/

int getButton(double resistance[], int len_resistance, int keyVal, int pastVal){
    // formula to get trigger is (5 - (5*r)/(r+10)) / 5 * 1023
    // with 10 being the second resistance (after button) and r the resistance in k Ohm

    // difference between two resistances must be at least 300 Ohm
    // diff = 10 / (1023/range - 1)
    // here range = 15
    int button = pastVal;

    for (int i=0; i<len_resistance; i++){
        int middleRange = (5 - (5*resistance[i]) / (resistance[i] + 10)) / 5 * 1023;
        if ( (keyVal > (middleRange - 15)) && (keyVal < (middleRange + 15)) ) button = i;
    }

    if (keyVal == 0) button = -1;

    return button;
}
