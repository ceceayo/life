/*
* Life. John Conway's famous game of life. Implementation by ceceayo
* Copyright (C) 2023 ceceayo

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <aalib.h>
#include <cstring>

#define on "x"
#define off " "

using namespace std;
unsigned int iterations = 0;


bool get_from_buffer(bool scr[], aa_context* context, int x, int y){
    if (x < 0) return false;
    if (y < 1) return false;
    if (x >= aa_scrwidth(context)) return false;
    if (y >= aa_scrheight(context)-2) return false;

    unsigned char z = scr[((y)* aa_scrwidth(context)) + (x)];
    if(scr[((y) * aa_scrwidth(context)) + (x)]){
        return true;
    } else {
        return false;
    }
    return 0;
}

uint8_t neighbours(bool scr[], aa_context* context, int x, int y){
    uint8_t value = 0;
    if (get_from_buffer(scr, context, x - 1, y - 1)) value ++;
    if (get_from_buffer(scr, context, x, y - 1)) value ++;
    if (get_from_buffer(scr, context, x+1, y-1)) value ++;
    if (get_from_buffer(scr, context, x-1, y)) value ++;
    if (get_from_buffer(scr, context, x+1, y)) value ++;
    if (get_from_buffer(scr, context, x-1, y+1)) value ++;
    if (get_from_buffer(scr, context, x, y+1)) value ++;
    if (get_from_buffer(scr, context, x+1, y+1)) value ++;
    return value;
}

void iterate(aa_context* context){
    bool scr[aa_scrwidth(context)* aa_scrheight(context)];
    for(int y=0; y != aa_scrheight(context); y++){
        for(int x=0; x != aa_scrwidth(context); x++){
            scr[(y* aa_scrwidth(context)+x)] = (aa_text(context)[(y* aa_scrwidth(context)+x)] == 'x');
            //cout << aa_text(context)[(y* aa_scrwidth(context)+x)] << endl;
        }
    }
    for(int y=1; y != aa_scrheight(context)-2; y++){
        for (int x=0; x != aa_scrwidth(context); x++){
            if (get_from_buffer(scr, context, x, y)) {
                switch (neighbours(scr, context, x, y)) {
                    case 0:
                    case 1:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        aa_puts(context, x, y, AA_NORMAL, off);
                        break;
                    case 2:
                    case 3:
                        aa_puts(context, x, y, AA_NORMAL, on);
                        break;
                }
            } else {
                if (neighbours(scr, context, x, y) == 3) aa_puts(context, x, y, AA_NORMAL, on);
            }
        }
    }
//    std::memcpy(&aa_text(context), &newscr, sizeof(newscr));
}


aa_context *context;
int cur_x;
int cur_y = 1;
int input;
int main(int argc, char **argv) {
    cout << "    ceceayo/life  Copyright (C) 2023  ceceayo\n"
            "    This program comes with ABSOLUTELY NO WARRANTY; for details see `LICENSE` in the source.\n"
            "    This is free software, and you are welcome to redistribute it\n"
            "    under certain conditions."<<endl;
    if(!aa_parseoptions(nullptr, nullptr, &argc, argv) || argc!=1) {
        printf("Usage: %s [options]\n"
               "Options:\n"
               "%s", argv[0], aa_help);
        return 2;
    }
    context = aa_autoinit(&aa_defparams);
    if(context == nullptr) {
        fprintf(stderr,"Cannot initialize AA-lib. Sorry\n");
        return 1;
    }
    aa_autoinitkbd(context, 1);
    aa_puts(context, 0, 0, AA_SPECIAL, "Game Of Life");
    aa_puts(context, aa_scrwidth(context)-4, 0, AA_SPECIAL, "v0.2");
    aa_puts(context, 20, 0, AA_SPECIAL, "I=0");
    aa_puts(context, 0, aa_scrheight(context)-2, AA_SPECIAL, "Z");
    aa_puts(context, 2, aa_scrheight(context)-2, AA_NORMAL, "iterate");
    aa_puts(context, 14, aa_scrheight(context)-2, AA_SPECIAL, "ARROWS");
    aa_puts(context, 21, aa_scrheight(context)-2, AA_NORMAL, "move cursor");
    aa_puts(context, 36, aa_scrheight(context)-2, AA_SPECIAL, ",");
    aa_puts(context, 38, aa_scrheight(context)-2, AA_NORMAL, "disable cell");
    aa_puts(context, 54, aa_scrheight(context)-2, AA_SPECIAL, ".");
    aa_puts(context, 56, aa_scrheight(context)-2, AA_NORMAL, "enable cell");
    aa_puts(context, 0, aa_scrheight(context)-1, AA_NORMAL, "Cells live with 2 or 3 neighbours, and get born with 3 neighbours");
    aa_puts(context, 16, aa_scrheight(context)-1, AA_SPECIAL, "2 or 3");
    aa_puts(context, 53, aa_scrheight(context)-1, AA_SPECIAL, "3");
    aa_gotoxy(context, 0, 1);
    aa_showcursor(context);
    while (true){
        aa_flush(context);
        input = aa_getkey(context, 0);
        if (input == 'q'){
            break;
        }
        switch (input) {
            case AA_LEFT:
                cur_x = ( cur_x - 1 );
                if (cur_x == -1) cur_x = aa_scrwidth(context);
                aa_gotoxy(context, cur_x, cur_y);
                break;
            case AA_RIGHT:
                cur_x = ( cur_x + 1 ) % aa_scrwidth(context);
                aa_gotoxy(context, cur_x, cur_y);
                break;
            case AA_UP:
                cur_y = ( cur_y - 1 ) ;
                if (cur_y == 0) cur_y = (aa_scrheight(context) - 3);
                aa_gotoxy(context, cur_x, cur_y);
                break;
            case AA_DOWN:
                cur_y = ( cur_y + 1 ) % (aa_scrheight(context) - 2);
                if (cur_y == 0) cur_y = 1;
                aa_gotoxy(context, cur_x, cur_y);
                break;
            case ',':
                aa_puts(context, cur_x, cur_y, AA_NORMAL, off);
                break;
            case '.':
                aa_puts(context, cur_x, cur_y, AA_BOLD, on);
                break;
            case 'z':
                iterate(context);
                iterations++;
                aa_puts(context, 22, 0, AA_SPECIAL, to_string(iterations).c_str());
                break;
            default:
                break;
        }
    }
    aa_uninitkbd(context);
    aa_close(context);
}