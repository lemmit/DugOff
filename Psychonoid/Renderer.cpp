#include "Renderer.h"


void CRenderer::Line(int startx, int starty, int endx, int endy, int color){
	Vector start(startx,starty);
	Vector end(endx,endy);
	Line(start,end,color);
}

void CRenderer::PolygonFilled(PolygonVertices* _polygon, int color) {
  if (_polygon->size()<3) return;

	StartDrawing();
  float miny, maxy;
  //Znajdujemy max i min na ykach
  miny = maxy = ((*_polygon)[0])->y;
  float temp_x1, temp_x2, temp_y1, temp_y2;
  for (unsigned i=0;i<_polygon->size();i++){
	temp_y1 = ((*_polygon)[i])->y;
    if (miny > temp_y1) {miny = temp_y1;}
    if (maxy < temp_y1) {maxy = temp_y1;}
  }

  //lista xów
  int *xlist = new int[_polygon->size()];
  unsigned xlistsize;

  // dla kazdego yka na wys polygona
  for (float y=miny;y<=maxy;y++) {
    // szukamy przeciec linii
    xlistsize = 0;
    for (unsigned i=0;i<_polygon->size();i++) {
      temp_y1 = ((*_polygon)[i])->y; 
	  temp_y2 = ((*_polygon)[(i+1)%(_polygon->size())])->y;
      if ((y>temp_y1 && y<=temp_y2) || (y>temp_y2 && y<=temp_y1)) {
        temp_x1 = ((*_polygon)[i])->x;
        temp_x2 = ((*_polygon)[(i+1)%(_polygon->size())])->x;
        xlist[xlistsize++] = (int)( temp_x1 + (y-temp_y1)*(temp_x2-temp_x1)/(temp_y2-temp_y1) );
      }
    }
    if (xlistsize==0) { continue; }
//sortujemy xy
	std::sort(xlist,xlist+xlistsize);
//malujemy co drugie
    for (int i=0;i<xlistsize;i+=2) {
      for (int x=xlist[i];x<=xlist[i+1];x++) {
		  PutPixel(x,y,color);
      }
    }
  }
  delete[] xlist;
  StopDrawing();
}


void CRenderer::Polygon(PolygonVertices* _polygon, int color){
	StartDrawing();
	for(int i =0; i<_polygon->size()-1;i++){
		Line(*((*_polygon)[i]),*((*_polygon)[i+1]), color);
	}
	Line(*((*_polygon)[_polygon->size()-1]), *((*_polygon)[0]), color);
	StopDrawing();
}
void CRenderer::ClearScreenToColor(int color){
	SDL_FillRect(screen, NULL, color);
}
void CRenderer::Circle(Vector center, int radius, int color){

		int *scr = (int *)screen->pixels;
		int pitch = screen->pitch / 4;
		int x,y;
		double tempdegs;
       for(int i=0;i<360;i++){
		   tempdegs=((double)i*M_PI/180);
		   x = center.x + (cos(tempdegs)*(double)radius); 
		   y = center.y + (sin(tempdegs)*(double)radius); 
				PutPixel(x,y,color);
	   }
}

void CRenderer::FlipScreen(){
		SDL_Flip(screen);
}

void CRenderer::PutPixel(int x, int y, int color){
	int *scr = (int *)screen->pixels;
	int pitch = screen->pitch / 4;
	while(x<0) x+=width;
	while(y<0) y+=height;
	scr[(x%width) + (y%height) * pitch] = color;
}

void CRenderer::Line(Vector p1, Vector p2, int color) {
	int dx, dy, de, err, i, x, y, t;
	int *scr = (int *)screen->pixels;
	int pitch = screen->pitch / 4;

	if(abs(p2.x - p1.x) > abs(p2.y - p1.y)) {
		if(p1.x > p2.x) {
			t = p1.x; p1.x = p2.x; p2.x = t;
			t = p1.y; p1.y = p2.y; p2.y = t;
			}

		t = p2.x - p1.x;
		de = abs(p2.y - p1.y);
		err = t / 2;
		if(p2.y > p1.y) dy = 1;
		else dy = -1;

		y = p1.y;
		for(i = p1.x; i <= p2.x; i++) {
			PutPixel(i,y,color);
			//scr[i + y * pitch] = color;
			err -= de;
			if(err < 0) {
				y += dy;
				err += t;
				}
			}
	} else {
		if(p1.y > p2.y) {
			t = p1.x; p1.x = p2.x; p2.x = t;
			t = p1.y; p1.y = p2.y; p2.y = t;
			}

		t = p2.y - p1.y;
		de = abs(p2.x - p1.x);
		err = t / 2;
		if(p2.x > p1.x) dx = 1;
		else dx = -1;

		x = p1.x;
		for(i = p1.y; i <= p2.y; i++) {
			PutPixel(x,i,color);
			//scr[x + i * pitch] = color;
			err -= de;
			if(err < 0) {
				x += dx;
				err += t;
				}
			}
		}
}

int CRenderer::InitFont(char* fontfile, int fontsize){
	tmpfont = TTF_OpenFont(fontfile, fontsize);
	if (tmpfont == NULL){
		printf("Unable to load font: %s %s \n", font, TTF_GetError());
		return 1;
	}
}
int CRenderer::DrawText(const char* text, int posx, int posy)
{
    SDL_Color color = {130,180,50};
    SDL_Surface *text_surface;
	SDL_Rect dest;
	
    text_surface = TTF_RenderText_Solid(tmpfont, text, color);
    if (text_surface != NULL)
    {	
		dest.x=posx;
		dest.y=posy;
		dest.w=text_surface->w;
		dest.h=text_surface->h;
        SDL_BlitSurface(text_surface, NULL, screen, &dest);
        SDL_FreeSurface(text_surface);
        return 1;
    }
    else
    {
        // report error
        return 0;
    }
	
}

void CRenderer::StartDrawing(){
	// zablokowanie ekranu, co umozliwia bezposrednie rysowanie
	SDL_LockSurface(screen);
}

void CRenderer::StopDrawing(){
	// po zakonczeniu rysowania nalezy ekran odblokowac
	SDL_UnlockSurface(screen);
}

CRenderer::CRenderer(int screen_width, int screen_height)
{
	screen = SDL_SetVideoMode(screen_width, screen_height, 32, SDL_HWSURFACE| SDL_SRCCOLORKEY | SDL_DOUBLEBUF);
	width = screen_width;
	height = screen_height;
	SDL_WM_SetCaption( "Psychonoid", 0 );
	// tryb pelnoekranowy
	//screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);

	if(screen == NULL) {
		printf("svm error: %s\n", SDL_GetError());
		return;
	}
	SDL_ShowCursor(SDL_DISABLE);
	font = SDL_LoadBMP("font.bmp");
	strings = SDL_LoadBMP("scorefps.bmp");
	if(font!=NULL) SDL_SetColorKey( font, SDL_SRCCOLORKEY, SDL_MapRGB(font->format, 255, 0, 255) );
	if(strings!=NULL) SDL_SetColorKey( strings, SDL_SRCCOLORKEY, SDL_MapRGB(font->format, 255, 0, 255) );
}

CRenderer::~CRenderer(void)
{
	 SDL_FreeSurface(font);
	 SDL_FreeSurface(strings);
}
