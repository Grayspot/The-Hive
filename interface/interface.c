#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include"interface.h"


void affichage_map(SDL_Renderer **renderer){
	char *map[] = {"img/HexBlankDusk.png","img/HexBlankDusk.png"};
	SDL_Surface *image[2];
	SDL_Rect dest_image[2];
	SDL_Texture *image_tex[2];
	SDL_RWops *rwop[2];

	for (int i=0; i<2; i++){
		rwop[i]=SDL_RWFromFile(map[i],"rb");
		image[i]=IMG_LoadPNG_RW(rwop[i]);
 		image_tex[i] = SDL_CreateTextureFromSurface(*renderer,image[i]);
	}

	dest_image[0].x=500;
	dest_image[0].y=200;
	dest_image[1].x=570;
	dest_image[1].y=220;
	for (int i=0;i<2;i++){
		SDL_QueryTexture(image_tex[i], NULL, NULL, &(dest_image[i].w), &(dest_image[i].h));
		SDL_RenderCopy(*renderer, image_tex[i], NULL, &dest_image[i]);
		SDL_FreeSurface(image[i]);
	}
}


int interface(){
  //Le pointeur vers la fenetre
	SDL_Window* ecran = NULL;
	//Le pointeur vers la surface incluse dans la fenetre
  SDL_Surface *texte_help=NULL, *texte_exit=NULL;
	SDL_Renderer *renderer=NULL;
	SDL_Rect rect1 = {0,0,400,900};
	SDL_Rect rect2 = {400,700,1200,200};
	SDL_Rect bouton_help = {5,705,390,90};
	SDL_Rect bouton_exit = {5,805,390,90};
	SDL_Rect dest_textHelp = bouton_help, dest_textExit = bouton_exit;

	// Le pointeur vers notre police
	TTF_Font *police = NULL;
	// Une variable de couleur verte
	SDL_Color couleurVerte = {63, 206, 10};

    /* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	/* Initialisation TTF */
	if(TTF_Init() == -1) {
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

  /* Création de la fenêtre */
	ecran = SDL_CreateWindow("The Hive",SDL_WINDOWPOS_UNDEFINED,  SDL_WINDOWPOS_UNDEFINED,1600,900, SDL_WINDOW_SHOWN);
	/* icone de la fenetre */
	SDL_Surface *icon=NULL;
	SDL_RWops *icon_rwop=NULL;
	icon_rwop=SDL_RWFromFile("img/ItmEncMergaLoupe.png","rb");
	icon=IMG_LoadPNG_RW(icon_rwop);
	SDL_SetWindowIcon(ecran,icon);

	if(!ecran){
		fprintf(stderr, "Erreur à la création de la fenetre : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

  renderer = SDL_CreateRenderer(ecran, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		fprintf(stderr, "Erreur à la création du renderer\n");
		exit(EXIT_FAILURE);
	}
	if( (police = TTF_OpenFont("Menlo-Regular.ttf", 55)) == NULL){
		fprintf(stderr, "erreur chargement font\n");
		exit(EXIT_FAILURE);
	}

	/* creation des textures pour afficher le texte */
	texte_help = TTF_RenderUTF8_Blended(police, " Help", couleurVerte);
	texte_exit = TTF_RenderUTF8_Blended(police, " Exit",couleurVerte);
	SDL_Texture *txt_texteHelp = SDL_CreateTextureFromSurface(renderer, texte_help);
	SDL_Texture *txt_texteExit = SDL_CreateTextureFromSurface(renderer, texte_exit);
	SDL_FreeSurface(texte_exit);
	SDL_FreeSurface(texte_help);
	SDL_QueryTexture(txt_texteHelp, NULL, NULL, &(dest_textHelp.w), &(dest_textHelp.h));
	SDL_QueryTexture(txt_texteExit, NULL, NULL, &(dest_textExit.w), &(dest_textExit.h));

  if( ecran ){
    int running = 1;
		while(running) {
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_QUIT: running = 0;
					break;
					case SDL_WINDOWEVENT:
						switch(event.window.event){
							case SDL_WINDOWEVENT_EXPOSED:
							case SDL_WINDOWEVENT_SIZE_CHANGED:
							case SDL_WINDOWEVENT_SHOWN:

				    SDL_RenderClear(renderer);

						/* affichage rectanglez verticale */
						SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
						SDL_RenderFillRect(renderer,&rect1);
						/* affichage rectangle horizontal */
						SDL_SetRenderDrawColor(renderer, 100, 90, 90, 255);
						SDL_RenderFillRect(renderer,&rect2);
						/* texte dans les boutons */
						SDL_RenderCopy(renderer, txt_texteHelp, NULL, &dest_textHelp);
						SDL_RenderCopy(renderer, txt_texteExit, NULL, &dest_textExit);
						/* les boutons */
						SDL_SetRenderDrawColor(renderer,200,200,200,255);
						SDL_RenderDrawRect(renderer,&bouton_exit);
						SDL_RenderDrawRect(renderer,&bouton_help);
						/* couleur du reste de la fenetre */
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						/*affichage de la carte */
						affichage_map(&renderer);

						SDL_RenderPresent(renderer);

							break;
						}
					break;
				}
			}
		}
  }
  else {
    fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
  }


  /****************************************************************************
  ***                         Destruction de la fenetre                     ***
  ****************************************************************************/
  SDL_DestroyWindow(ecran);

	TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
	TTF_Quit();
  SDL_Quit();
	return 0;
}

int main(){
	interface();
}
