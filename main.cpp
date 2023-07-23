#include <iostream>
#include <aalib.h>
#include <cstring>

#define on "x"
#define off " "

using namespace std;



bool get_from_buffer(bool scr[], aa_context* context, int x, int y){
    if (x < 0) return false;
    if (y < 0) return false;
    if (x >= aa_scrwidth(context)) return false;
    if (y >= aa_scrheight(context)) return false;

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
    for(int y=0; y != aa_scrheight(context); y++){
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
                        aa_puts(context, x, y, AA_SPECIAL, on);
                        break;
                }
            } else {
                if (neighbours(scr, context, x, y) == 3) aa_puts(context, x, y, AA_SPECIAL, on);
            }
        }
    }
//    std::memcpy(&aa_text(context), &newscr, sizeof(newscr));
}


aa_context *context;
int cur_x, cur_y;
int input;
int main(int argc, char **argv) {
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
    aa_autoinitkbd(context, 0);
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
                if (cur_y == -1) cur_y = aa_scrheight(context);
                aa_gotoxy(context, cur_x, cur_y);
                break;
            case AA_DOWN:
                cur_y = ( cur_y + 1 ) % aa_scrheight(context);
                aa_gotoxy(context, cur_x, cur_y);
                break;
            case ',':
                aa_puts(context, cur_x, cur_y, AA_NORMAL, off);
                break;
            case '.':
                aa_puts(context, cur_x, cur_y, AA_SPECIAL, on);
                break;
            case 'z':
                iterate(context);
                break;
            default:
                break;
        }
    }
    aa_uninitkbd(context);
    aa_close(context);
}