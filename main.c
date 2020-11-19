#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#define MAX_N 50 // nokta limiti

#define PI 3.14159265

//cember icin struct yapisi
typedef struct{
  double x, y;
  double r;
} circle;

   //iki nokta arasi uzaklik
  double calculateDistance(double x1, double y1, double x2, double y2){
  double distance;
  distance = sqrt(pow(x1-x2, 2) + pow(y1 -y2, 2));
  return distance;
}


circle bestCircle_bul(double x1, double y1, double x2, double y2, double x3, double y3) {

    circle bestCircle;

    double x12 = x1 - x2;
    double x13 = x1 - x3;
    double x31 = x3 - x1;
    double x21 = x2 - x1;

    double y12 = y1 - y2;
    double y13 = y1 - y3;
    double y31 = y3 - y1;
    double y21 = y2 - y1;

    double sx13 = pow(x1, 2) - pow(x3, 2); // x1^2 - x3^2

    double sy13 = pow(y1, 2) - pow(y3, 2);  // y1^2 - y3^2

    double sx21 = pow(x2, 2) - pow(x1, 2);

    double sy21 = pow(y2, 2) - pow(y1, 2);

    double f = ((sx13) * (x12) + (sy13) * (x12) + (sx21) * (x13) + (sy21) * (x13))
            / (2 * ((y31) * (x12) - (y21) * (x13)));

    double g = ((sx13) * (y12) + (sy13) * (y12) + (sx21) * (y13) + (sy21) * (y13))
            / (2 * ((x31) * (y12) - (x21) * (y13)));

    double c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

    // x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // merkez (h = -g, k = -f)
    // r^2 = h^2 + k^2 - c
    double h = -g;
    double k = -f;
    double r2 = h * h + k * k - c;

    // r
    double r = sqrt(r2);

    bestCircle.x = h;
    bestCircle.y = k;
    bestCircle.r = r;
    return bestCircle;
}


circle MinimumEnclosingCircle2(double points[][2], int n, int first_point_index, int second_point_index){
  circle c;
  float inCircle;

  c.x = (points[first_point_index][0] + points[second_point_index][0]/2);
  c.y = (points[first_point_index][1] + points[second_point_index][1]/2);
  c.r = (calculateDistance(points[first_point_index][0], points[first_point_index][1], points[second_point_index][0], points[second_point_index][1])/2);

  for(int i=0; i<n; i++){  //cember icinde olmayan ilk noktanin indisine kadar
    inCircle = pow(points[i][0] - c.x, 2) + pow(points[i][1] - c.y, 2) - pow(c.r, 2);

    if(inCircle > 0) { //cember icinde degil ise
      c = bestCircle_bul(points[first_point_index][0], points[first_point_index][1], points[second_point_index][0], points[second_point_index][1], points[i][0], points[i][1]);
    }
  }

  return c;
}


circle MinimumEnclosingCircle1(double points[][2], int point_index, int n){
  circle c;
  int j;
  double inCircle;


  c.x = (points[point_index][0]+points[0][0])/2; //ilk çemberin içinde olmayan noktax ile ilknoktax
  c.y = (points[point_index][1]+points[0][1])/2;
  c.r = (calculateDistance(points[point_index][0], points[point_index][1], points[0][0], points[0][1])/2);


  for(j=1; j<point_index; j++){ //cember icinde olmayan ilk noktanin indisine kadar
    inCircle = pow(points[j][0] - c.x, 2) + pow(points[j][1] - c.y, 2) - pow(c.r, 2);
    if(inCircle > 0){ //cember icinde degil ise

      c = MinimumEnclosingCircle2(points, j, j, point_index);
    }
  }

  return c;

}

//algorithm complexity O(n)
circle MinimumEnclosingCircle0(double points[][2], int n){
  circle c;
  double inCircle;

  c.x = (points[1][0]+points[0][0])/2;
  c.y = (points[1][1]+points[0][1])/2;
  c.r = (calculateDistance(points[0][0], points[0][1], points[1][0], points[1][1])/2);

  for(int i=2; i<n; i++){

        // (x-cx)^2 + (y-cy)^2 < r^2 --->  (x-cx)^2 + (y-cy)^2 - r^2 < 0 (Nokta çemberin içinde)
    inCircle = pow(points[i][0] - c.x, 2) + pow(points[i][1] - c.y, 2) - pow(c.r, 2);
    if(inCircle > 0){ //nokta çemberin içinde deðil ise
      c = MinimumEnclosingCircle1(points, i, n);
    }
  }

  return c;
}

int main(int argc, char **argv) {

  FILE *f;
  int N; //txt'deki nokta sayisi
  double points[MAX_N][2]; //x, y
  int i;
  circle c;

            ALLEGRO_DISPLAY *display = NULL;
            ALLEGRO_BITMAP  *image = NULL;

               // Initialize Allegro Library
            if(!al_init()) {
                fprintf(stderr, "failed to initialize allegro\n");
                return -1;
            }
            // Create Display
            display = al_create_display(800,800); //ekran boyutu
            if (!display) {
                fprintf(stderr, "failed to create display\n");
                return -1;
            }
            al_set_window_position(display,200,200); //ekranin pozisyonu
            al_set_window_title(display,"Minimum Enclosing Circle"); //ekranin adi
            al_clear_to_color(al_map_rgb(255,255,255)); //ekran rengi

            al_init_primitives_addon();
            al_init_font_addon();
            al_init_ttf_addon();
            al_init_image_addon();

            ALLEGRO_FONT *sec = al_load_ttf_font("comic.ttf", 24, 0);
            al_draw_text(sec, al_map_rgb(119,136,153), 400, 400, ALLEGRO_ALIGN_CENTRE, "UYGULAMAMIZA HOSGELDINIZ");
            al_flip_display();

            int secim = 0;
            printf("UYGULAMAMIZA HOSGELDINIZ: \n");
            printf("1-N NOKTAYI ICINE ALAN EN KUCUK CEMBER-MEC\n");
            scanf("%d", &secim);


             f = fopen("test2.txt", "r");
              fscanf(f, "%d", &N);
              printf(".TXT DOSYASINDAKI NOKTA SAYISI:\n ");
              printf("%d\n",N); //dosyadaki satir sayisi
              printf("\n");

              for(i=0;i<N;i++){
                fscanf(f, "%lf %lf", &points[i][0], &points[i][1]);
              }


               printf(".TXT DOSYASINDAN OKUNAN NOKTALAR:\n");
               for(i=0;i<N;i++){ //koordinat noktalarini yazdirir
                printf("%lf %lf \n", points[i][0], points[i][1]);
              }
                printf("\n");

                image = al_load_bitmap("background.jpg");
                al_draw_bitmap(image, 0, 0, 0);

//****************************************************************************************************
                //X KOORDINAT EKSENI
                al_draw_line(0, 400, 800,400, al_map_rgb(105,105,105),3.0);
                //Y KOORDINAT EKSENI
                al_draw_line(400, 0, 400,800, al_map_rgb(105,105,105),3.0);
                al_flip_display();

//****************************************************************************************************
                //KOORDINAT DUZLEMINE (1,2,3.....)
                 ALLEGRO_FONT *font = al_load_ttf_font("comic.ttf", 10, 0);
                 //x icin +
                 al_draw_text(font, al_map_rgb(105,105,105), 420, 400, ALLEGRO_ALIGN_CENTRE, "1");
                 al_draw_text(font, al_map_rgb(105,105,105), 440, 400, ALLEGRO_ALIGN_CENTRE, "2");
                 al_draw_text(font, al_map_rgb(105,105,105), 460, 400, ALLEGRO_ALIGN_CENTRE, "3");
                 al_draw_text(font, al_map_rgb(105,105,105), 480, 400, ALLEGRO_ALIGN_CENTRE, "4");
                 al_draw_text(font, al_map_rgb(105,105,105), 500, 400, ALLEGRO_ALIGN_CENTRE, "5");
                 al_draw_text(font, al_map_rgb(105,105,105), 520, 400, ALLEGRO_ALIGN_CENTRE, "6");
                 al_draw_text(font, al_map_rgb(105,105,105), 540, 400, ALLEGRO_ALIGN_CENTRE, "7");
                 al_draw_text(font, al_map_rgb(105,105,105), 560, 400, ALLEGRO_ALIGN_CENTRE, "8");
                 al_draw_text(font, al_map_rgb(105,105,105), 580, 400, ALLEGRO_ALIGN_CENTRE, "9");
                 al_draw_text(font, al_map_rgb(105,105,105), 600, 400, ALLEGRO_ALIGN_CENTRE, "10");
                 al_draw_text(font, al_map_rgb(105,105,105), 620, 400, ALLEGRO_ALIGN_CENTRE, "11");
                 al_draw_text(font, al_map_rgb(105,105,105), 640, 400, ALLEGRO_ALIGN_CENTRE, "12");
                 al_draw_text(font, al_map_rgb(105,105,105), 660, 400, ALLEGRO_ALIGN_CENTRE, "13");
                 al_draw_text(font, al_map_rgb(105,105,105), 680, 400, ALLEGRO_ALIGN_CENTRE, "14");
                 al_draw_text(font, al_map_rgb(105,105,105), 700, 400, ALLEGRO_ALIGN_CENTRE, "15");
                 al_draw_text(font, al_map_rgb(105,105,105), 720, 400, ALLEGRO_ALIGN_CENTRE, "16");
                 al_draw_text(font, al_map_rgb(105,105,105), 740, 400, ALLEGRO_ALIGN_CENTRE, "17");
                 al_draw_text(font, al_map_rgb(105,105,105), 760, 400, ALLEGRO_ALIGN_CENTRE, "18");
                 al_draw_text(font, al_map_rgb(105,105,105), 780, 400, ALLEGRO_ALIGN_CENTRE, "19");
                 al_draw_text(font, al_map_rgb(105,105,105), 800, 400, ALLEGRO_ALIGN_CENTRE, "20");
                 al_flip_display();
                // x icin -
                 al_draw_text(font, al_map_rgb(105,105,105), 380, 400, ALLEGRO_ALIGN_CENTRE, "-1");
                 al_draw_text(font, al_map_rgb(105,105,105), 360, 400, ALLEGRO_ALIGN_CENTRE, "-2");
                 al_draw_text(font, al_map_rgb(105,105,105), 340, 400, ALLEGRO_ALIGN_CENTRE, "-3");
                 al_draw_text(font, al_map_rgb(105,105,105), 320, 400, ALLEGRO_ALIGN_CENTRE, "-4");
                 al_draw_text(font, al_map_rgb(105,105,105), 300, 400, ALLEGRO_ALIGN_CENTRE, "-5");
                 al_draw_text(font, al_map_rgb(105,105,105), 280, 400, ALLEGRO_ALIGN_CENTRE, "-6");
                 al_draw_text(font, al_map_rgb(105,105,105), 260, 400, ALLEGRO_ALIGN_CENTRE, "-7");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 240, 400, ALLEGRO_ALIGN_CENTRE, "-8");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 220, 400, ALLEGRO_ALIGN_CENTRE, "-9");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 200, 400, ALLEGRO_ALIGN_CENTRE, "-10");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 180, 400, ALLEGRO_ALIGN_CENTRE, "-11");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 160, 400, ALLEGRO_ALIGN_CENTRE, "-12");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 140, 400, ALLEGRO_ALIGN_CENTRE, "-13");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 120, 400, ALLEGRO_ALIGN_CENTRE, "-14");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 100, 400, ALLEGRO_ALIGN_CENTRE, "-15");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 80, 400, ALLEGRO_ALIGN_CENTRE, "-16");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 60, 400, ALLEGRO_ALIGN_CENTRE, "-17");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 40, 400, ALLEGRO_ALIGN_CENTRE, "-18");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 20, 400, ALLEGRO_ALIGN_CENTRE, "-19");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 0, 400, ALLEGRO_ALIGN_CENTRE, "-20");
                 al_flip_display();
                //y icin +
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 375, ALLEGRO_ALIGN_CENTRE, "1");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 355, ALLEGRO_ALIGN_CENTRE, "2");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 335, ALLEGRO_ALIGN_CENTRE, "3");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 315, ALLEGRO_ALIGN_CENTRE, "4");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 295, ALLEGRO_ALIGN_CENTRE, "5");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 275, ALLEGRO_ALIGN_CENTRE, "6");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 255, ALLEGRO_ALIGN_CENTRE, "7");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 235, ALLEGRO_ALIGN_CENTRE, "8");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 215, ALLEGRO_ALIGN_CENTRE, "9");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 195, ALLEGRO_ALIGN_CENTRE, "10");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 175, ALLEGRO_ALIGN_CENTRE, "11");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 155, ALLEGRO_ALIGN_CENTRE, "12");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 135, ALLEGRO_ALIGN_CENTRE, "13");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 115, ALLEGRO_ALIGN_CENTRE, "14");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 95, ALLEGRO_ALIGN_CENTRE, "15");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 75, ALLEGRO_ALIGN_CENTRE, "16");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 55, ALLEGRO_ALIGN_CENTRE, "17");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 35, ALLEGRO_ALIGN_CENTRE, "18");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 15, ALLEGRO_ALIGN_CENTRE, "19");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, -5, ALLEGRO_ALIGN_CENTRE, "20");
                 al_flip_display();
                //y icin -
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 410, ALLEGRO_ALIGN_CENTRE, "-1");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 430, ALLEGRO_ALIGN_CENTRE, "-2");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 450, ALLEGRO_ALIGN_CENTRE, "-3");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 470, ALLEGRO_ALIGN_CENTRE, "-4");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 490, ALLEGRO_ALIGN_CENTRE, "-5");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 510, ALLEGRO_ALIGN_CENTRE, "-6");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 530, ALLEGRO_ALIGN_CENTRE, "-7");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 550, ALLEGRO_ALIGN_CENTRE, "-8");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 570, ALLEGRO_ALIGN_CENTRE, "-9");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 590, ALLEGRO_ALIGN_CENTRE, "-10");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 610, ALLEGRO_ALIGN_CENTRE, "-11");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 630, ALLEGRO_ALIGN_CENTRE, "-12");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 650, ALLEGRO_ALIGN_CENTRE, "-13");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 670, ALLEGRO_ALIGN_CENTRE, "-14");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 690, ALLEGRO_ALIGN_CENTRE, "-15");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 710, ALLEGRO_ALIGN_CENTRE, "-16");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 730, ALLEGRO_ALIGN_CENTRE, "-17");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 750, ALLEGRO_ALIGN_CENTRE, "-18");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 770, ALLEGRO_ALIGN_CENTRE, "-19");
                 al_draw_text(font, al_map_rgb(105, 105, 105), 410, 790, ALLEGRO_ALIGN_CENTRE, "-20");
                 al_flip_display();


//****************************************************************************************************

            //KOORDINAT NOKTALARINI EKRANDA GOSTERME
                for(i=0;i<N;i++){
                    double a_x = points[i][0]; // x degeri
                    double b_y = points[i][1]; //y degeri

                    if(a_x>=0 && b_y>=0){  //koordinatlar 1.bölgede(+,+)

                       int a_x1 = (int) 400 + 20* points[i][0];
                       int b_y1 =(int) 400 - 20* points[i][1];
                       //printf("%d",a_x1);
                       //printf("\n");
                       al_draw_line(a_x1,400, a_x1,b_y1, al_map_rgb(112,128,144),1.0);
                       al_draw_line(400, b_y1, a_x1,b_y1, al_map_rgb(112,128,144),1.0);
                       al_draw_filled_circle(a_x1,b_y1,3,al_map_rgb(112,128,144));
                       al_flip_display();
                    }


                     else if(a_x<=0 && b_y>=0){ //koordinatlar 2.bölgede(-,+)

                       int a_x2 = (int) 400 - (20* (-1) * points[i][0]);
                       int b_y2 =(int) 400 - 20* points[i][1];
                       //printf("%d",a_x1);
                       //printf("\n");
                       al_draw_line(a_x2,400, a_x2,b_y2, al_map_rgb(112,128,144),1.0);
                       al_draw_line(400, b_y2, a_x2,b_y2, al_map_rgb(112,128,144),1.0);
                       al_draw_filled_circle(a_x2,b_y2,3,al_map_rgb(112,128,144));
                       al_flip_display();

                    }

                      else if(a_x<=0 && b_y<=0){ //koordinatlar 3.bölgede(-,-)

                       int a_x3 = (int) 400 - (20* (-1) * points[i][0]);
                       int b_y3 =(int) 400 + (20* (-1) * points[i][1]);
                       //printf("%d",a_x1);
                       //printf("\n");
                       al_draw_line(a_x3,400, a_x3,b_y3, al_map_rgb(112,128,144),1.0);
                       al_draw_line(400, b_y3, a_x3,b_y3, al_map_rgb(112,128,144),1.0);
                       al_draw_filled_circle(a_x3,b_y3,3,al_map_rgb(112,128,144));
                       al_flip_display();

                    }

                      else if (a_x>=0 && b_y<=0){ //koordinatlar 4.bölgede(+,-)

                       int a_x4 = (int) 400 + 20* points[i][0];
                       int b_y4 =(int) 400 + (20* (-1) * points[i][1]);
                       //printf("%d",a_x1);
                       //printf("\n");
                       al_draw_line(a_x4,400, a_x4,b_y4, al_map_rgb(112,128,144),1.0);
                       al_draw_line(400, b_y4, a_x4,b_y4, al_map_rgb(112,128,144),1.0);
                       al_draw_filled_circle(a_x4,b_y4,3,al_map_rgb(112,128,144));
                       al_flip_display();

                    }
                  }



//***********************************************************************************************************
                    //NOKTALARI ICINE ALAN CEMBERIN MERKEZI VE YARICAPI

                   c = MinimumEnclosingCircle0(points, N);
                   printf("TUM NOKTALARI ICEREN CEMBER BILGILERI:\n");
                   printf("MERKEZI --> (%.2lf, %.2lf), YARICAPI --> %.4lf\n", c.x, c.y, c.r);
                   printf("\n");

//************************************************************************************************************
                     //NOKTALARI ICINE ALAN CEMBERIN EKRANDA GOSTERIMI

                     if(c.x>=0 && c.y>=0){  //merkez 1.bölgede(+,+)

                       int c_x1 = (int) 400 + 20* c.x;
                       int c_y1 =(int) 400 - 20* c.y;
                       int c_r =(int) 20* c.r;
                        al_draw_circle(c_x1,c_y1,c_r, al_map_rgb(47,79,79),2.0);
                        al_flip_display();

                        //CEMBERÝN MERKEZ GOSTERIMI
                        al_draw_filled_circle(c_x1,c_y1,4, al_map_rgb(47,79,79));
                        al_flip_display();

                    }


                     else if(c.x<=0 && c.y>=0){ //merkez 2.bölgede(-,+)

                       int c_x2 = (int) 400 - (20* (-1) * c.x);
                       int c_y2 =(int) 400 - 20* c.y;
                       int c_r =(int) 20* c.r;
                       al_draw_circle(c_x2,c_y2,c_r, al_map_rgb(47,79,79),2.0);
                       al_flip_display();

                       //CEMBERÝN MERKEZ GOSTERIMI
                       al_draw_filled_circle(c_x2,c_y2,4, al_map_rgb(47,79,79));
                       al_flip_display();

                    }

                      else if(c.x<=0 && c.y<=0){ //merkez 3.bölgede(-,-)

                       int c_x3 = (int) 400 - (20* (-1) * c.x);
                       int c_y3 =(int) 400 + (20* (-1) * c.y);
                       int c_r =(int) 20* c.r;
                       al_draw_circle(c_x3,c_y3,c_r, al_map_rgb(47,79,79),2.0);
                       al_flip_display();

                       //CEMBERÝN MERKEZ GOSTERIMI
                       al_draw_filled_circle(c_x3,c_y3,4, al_map_rgb(47,79,79));
                       al_flip_display();

                    }

                      else if (c.x>=0 && c.y<=0){ //merkez 4.bölgede(+,-)

                       double c_x4 =  400 + 20* c.x;
                       double c_y4 = 400 + (20* (-1) * c.y);
                       double c_r = 20* c.r;
                       al_draw_circle(c_x4,c_y4,c_r, al_map_rgb(47,79,79),2.0);
                       al_flip_display();

                       //CEMBERIN MERKEZ GOSTERIMI
                       al_draw_filled_circle(c_x4,c_y4,4, al_map_rgb(47,79,79));
                       al_flip_display();

                    }
//************************************************************************************************************
                    //CEMBERIN YARICAP CIZIMI
                    double derece,val;
                    derece = 0;
                    val = PI / 180;
                    double on_x = c.x + c.r * cos(derece*val);
                    double on_y = c.y + c.r * sin(derece*val);
                    //printf("%lf %lf \n",on_x, on_y); //Cember uzerindeki bir nokta koordinati
                      if(c.x>=0 && c.y>=0){  //merkez 1.bölgede(+,+)

                       int c_xkoor = (int) 400 + 20* c.x;
                       int c_ykoor =(int) 400 - 20* c.y;

                                           if(on_x>=0 && on_y>=0){  //koordinatlar 1.bölgede(+,+)

                                           int on_xkoor = (int) 400 + 20* on_x;
                                           int on_ykoor =(int) 400 - 20* on_y;
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();
                                        }

                                         else if(on_x<=0 && on_y>=0){ //koordinatlar 2.bölgede(-,+)

                                           int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                           int on_ykoor =(int) 400 - 20* on_y;
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                          else if(on_x<=0 && on_y<=0){ //koordinatlar 3.bölgede(-,-)

                                           int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                           int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                          else if (on_x>=0 && on_y<=0){ //koordinatlar 4.bölgede(+,-)

                                           int on_xkoor = (int) 400 + 20* on_x;
                                           int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }
                                     }


                     else if(c.x<=0 && c.y>=0){ //merkez 2.bölgede(-,+)

                       int c_xkoor = (int) 400 - (20* (-1) * c.x);
                       int c_ykoor =(int) 400 - 20* c.y;
                                           if(on_x>=0 && on_y>=0){  //koordinatlar 1.bölgede(+,+)

                                           int on_xkoor = (int) 400 + 20* on_x;
                                           int on_ykoor =(int) 400 - 20* on_y;
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();
                                        }

                                         else if(on_x<=0 && on_y>=0){ //koordinatlar 2.bölgede(-,+)

                                           int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                           int on_ykoor =(int) 400 - 20* on_y;
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                          else if(on_x<=0 && on_y<=0){ //koordinatlar 3.bölgede(-,-)

                                           int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                           int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                          else if (on_x>=0 && on_y<=0){ //koordinatlar 4.bölgede(+,-)

                                           int on_xkoor = (int) 400 + 20* on_x;
                                           int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                    }

                      else if(c.x<=0 && c.y<=0){ //merkez 3.bölgede(-,-)

                       int c_xkoor = (int) 400 - (20* (-1) * c.x);
                       int c_ykoor =(int) 400 + (20* (-1) * c.y);
                                                if(on_x>=0 && on_y>=0){  //koordinatlar 1.bölgede(+,+)

                                                   int on_xkoor = (int) 400 + 20* on_x;
                                                   int on_ykoor =(int) 400 - 20* on_y;
                                                   al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                                   al_flip_display();
                                                }

                                                 else if(on_x<=0 && on_y>=0){ //koordinatlar 2.bölgede(-,+)

                                                   int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                                   int on_ykoor =(int) 400 - 20* on_y;
                                                   al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                                   al_flip_display();

                                                }

                                                  else if(on_x<=0 && on_y<=0){ //koordinatlar 3.bölgede(-,-)

                                                   int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                                   int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                                   al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                                   al_flip_display();

                                                }

                                                  else if (on_x>=0 && on_y<=0){ //koordinatlar 4.bölgede(+,-)

                                                   int on_xkoor = (int) 400 + 20* on_x;
                                                   int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                                   al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                                   al_flip_display();

                                                }
                                            }

                      else if (c.x>=0 && c.y<=0){ //merkez 4.bölgede(+,-)

                       double c_xkoor =  400 + 20* c.x;
                       double c_ykoor = 400 + (20* (-1) * c.y);
                                          if(on_x>=0 && on_y>=0){  //koordinatlar 1.bölgede(+,+)

                                           int on_xkoor = (int) 400 + 20* on_x;
                                           int on_ykoor =(int) 400 - 20* on_y;
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();
                                        }

                                         else if(on_x<=0 && on_y>=0){ //koordinatlar 2.bölgede(-,+)

                                           int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                           int on_ykoor =(int) 400 - 20* on_y;
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                          else if(on_x<=0 && on_y<=0){ //koordinatlar 3.bölgede(-,-)

                                           int on_xkoor = (int) 400 - (20* (-1) * on_x);
                                           int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }

                                          else if (on_x>=0 && on_y<=0){ //koordinatlar 4.bölgede(+,-)

                                           int on_xkoor = (int) 400 + 20* on_x;
                                           int on_ykoor =(int) 400 + (20* (-1) * on_y);
                                           al_draw_line(c_xkoor,c_ykoor,on_xkoor,on_ykoor, al_map_rgb(47,79,79),2.0);
                                           al_flip_display();

                                        }


                                        }







    al_rest(120);  //ekranin görünme süresi
    al_destroy_display(display); //ekran yok etme

    return 0;
}
