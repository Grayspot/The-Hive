#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lib/structure.h"

/**
 * \file game.c
 * \brief Fonctions essentielles au déroulement du jeu
 * \author Mathilde Mottay, Anais Mottier, Clement Mainguy, Moustapha Tsamarayev
 * \version 1.0
 * \date 2020
*/


/**
 * \fn void presentation_regle_jeu()
 * \brief Présentation du jeu.
 * \return Rien
*/
void presentation_regle_jeu(){
  while(getchar() != '\n');
  clrscr();
  printf("\n   EXPLICATIONS DES REGLES DU JEU SUR CETTE PAGE \n\n");
  printf("\n   Appuyez sur la touche entrée pour commencer l'aventure ! ");
  while(getchar() != '\n');
}


/**
 * \fn int exit_game()
 * \brief Propose au joueur de quitter ou non la carte lorsqu'il vient de trouver la sortie.
 * \return Un \a int : 1 si le joueur décide de quitter la carte. 0 s'il décide de continuer l'aventure.
*/
int exit_game(){
    int rep;

    printf("   Vous venez de trouver la sortie ! Souhaitez-vous quitter la map ? (Oui = 1, Non = 0)\n");
    scanf("%d", &rep);
    if (rep == 0){
        printf ("   Vous êtes courageux ! L'aventure continue !\n");
        return 0;
    }
    else {
        printf("   Félicitation pour votre parcours, vous pouvez maintenant vivre sans craindre pour votre vie ! \n A bientôt peut-être, pour de nouvelles aventures !\n");
        return 1;
    }
}


/**
 * \fn void menu_principal_jeu(perso_t player, cell_t map[D][D], int quest_map[6][2], quete_t quete, sauv_t sauv, item_t * Tab_Items, int nb_items_available)
 * \brief Affichage du menu principal du jeu et déroulement de toute la partie
 * \param perso_t player
 * \param cell_t map[D][D]
 * \param int quest_map[6][2]
 * \param quete_t quete
 * \param sauv_t sauv
 * \param item_t * Tab_Items
 * \param int nb_items_available
 * \return Rien
*/
void menu_principal_jeu(perso_t player, cell_t map[D][D], int quest_map[6][2], quete_t quete, sauv_t sauv, item_t * Tab_Items, int nb_items_available){
  int choise;
  int choix_combat;

  /* Pour combat */
  npc_t * enemy;
  stat_t * field;

  clrscr();

  while((player.turns != 0) && (player.pv != 0)){

    /*Si il y  une quête sur l'hexagone*/
    if(map[player.posX][player.posY].quest_id != 0){
        quetes(&player, map, quest_map, &quete, Tab_Items, nb_items_available);
    }
    /* GESTION COMBAT - VOIR AVEC MOUSTAPHA */
    if(map[player.posX][player.posY].encounter){
      field=init_field();
      enemy=init_npc(Tab_Items);
      printf("\n   Vous tombez nez à nez avec un %s!\n\n   Souhaitez vous le combattre ? (Oui : 1, Non : 0)\n", enemy->name);
      printf("   Votre réponse : ");
      do {
        scanf("%d",&choix_combat);
        if(choix_combat < 0 || choix_combat > 1){
          printf("Le choix est pourtant simple. Veuillez ressaisir : ");
        }
      } while(choix_combat < 0 || choix_combat > 1);

      if(choix_combat){
        if(combat(&player, enemy, field, Tab_Items, nb_items_available)==1){ // TO BE TESTED
            random_move(&player,map);
        }
      }
      else{
        printf("\n   Courage fuyons, vous prenez la poudre d'escampette!\n");
        sleep(2);

      }
      map[player.posX][player.posY].encounter = 0;
      clrscr();
    }

    display_player_characteristics(map, player);

    printf ("\033[34;01m   [Menu principal]\033[00m\n");
    printf("    1 - Fouiller la zone\n");
    printf("    2 - Gérer l'inventaire\n");
    printf("    3 - Gérer l'équipement\n");
    printf("    4 - Se déplacer ailleurs\n");
    printf("    5 - Pêcher\n");
    printf("    6 - Regarder la carte (carte nécessaire)\n");
    printf("    7 - Se reposer et guérir\n");
    printf("    8 - Fin du tour\n");
    printf("    9 - Sauvegarder le jeu et quitter\n");
    printf("    10 - Aide\n");
    //Si la quete "recherche" est en cours + joueur cherche l'item demande par l'homme
    if(quete.recherche.trouve == 0)
        printf("    11 - Rechercher l'item %s demandé par l'homme\n", quete.recherche.wanted.name);
    //Si la quete "recherche" est en cours + joueur a trouvé l'item demandé par l'homme
    if((quete.recherche.trouve == 1) && (quete.recherche.situation == 0))
        printf("    11 - Retrouver l'homme pour lui donner l'item %s\n", quete.recherche.wanted.name);
    //Si la quete "recherche" est fini
    if(quete.recherche.situation == 1)
        printf("    11 - Voir les coordonnées du lieu sécurisé donné par l'homme\n");
    printf("\n   Quitter sans sauvegarder : -1\n\n");
    printf("   Que souhaitez-vous faire ? ");

    jump:
    scanf("%d",&choise);
    switch(choise){
      case 1: clrscr(); scavenge(map,&player,Tab_Items,nb_items_available); clrscr(); break;
      case 2: clrscr(); manage_inventory(&player); clrscr(); break;
      case 3: clrscr(); manage_equipment(&player); clrscr(); break;
      case 4: clrscr(); move(&player, map); clrscr(); break;
      case 5: clrscr(); fish(&player,map); clrscr(); break;
      case 6: clrscr(); check_the_map(player, map); clrscr(); break;
      case 7: clrscr(); rest_and_heal(&player); clrscr(); break;
      case 8: clrscr(); next_turn(&player); clrscr(); break;
      case 9: clrscr(); save(player,map,quest_map,quete,sauv); exit(1); break;
      case 10: clrscr(); help(&player); clrscr(); break;
      case 11: clrscr();
               if(quete.recherche.situation == 0)
                    quete_recherche(&player, map, &quete, quest_map, Tab_Items, nb_items_available);
               else
                    printf("Coordonnées du lieu sécurisé : X = %d, Y = %d\n", quete.recherche.bunkerX, quete.recherche.bunkerY);
               clrscr(); break;
//    case 12: display_quest(quest_map); sleep(4); break;
      case -1: exit(1); break;
      default: printf("   Commande inconnue. Veuillez ressaisir: "); goto jump; break;
    }
  }

  if(!player.turns){
    printf("   Malheureusement, vous n'avez pas eu le temps de vous échapper...\n");
  }
  if(!player.pv){
    printf("   Vous êtes mort.\n");
  }


  free(enemy);
  free(field);
  free(Tab_Items);
  enemy=NULL;
  field=NULL;
  Tab_Items=NULL;

  sleep(2);
  clrscr();
}


/**
 * \fn void choix_partie(sauv_t * sauv)
 * \brief Choix de la partie sauvegardé à jouer
 * \param sauv_t sauv
 * \return Rien
*/
void choix_partie(sauv_t * sauv){
  int choix;
  char nom_partie[21];

  perso_t player;
  cell_t map[D][D];
  int quest_map[6][2];
  quete_t quete;
  item_t * Tab_Items = malloc(20 * sizeof(item_t));
  int nb_items_available = 0;

  clrscr();
  affichage_parties(*sauv);
  printf("   A quelle partie souhaitez-vous jouer ?\n");
  printf("   Numéro partie : ");
  do {
    scanf("%d",&choix);
    if((choix < 1) || (choix > 3)){
      printf("   Valeur incorrecte. Veuillez ressaisir : ");
    }
  } while ((choix < 1) || (choix > 3));

  sauv->numPartie = choix;
  if(sauvegarde_existante(*sauv)){
      load(&player,map,quest_map,&quete,*sauv);
  }
  else {
    printf("\n   Quel nom souhaitez-vous donner à votre partie ? (20 caractères max)\n   ");
    scanf(" %[^\n]",nom_partie);

    switch(sauv->numPartie){
     case 1: strcpy(sauv->nomPartie1,nom_partie); break;
     case 2: strcpy(sauv->nomPartie2,nom_partie); break;
     case 3: strcpy(sauv->nomPartie3,nom_partie); break;
     default: break;
    }

    init_player(&player);
    map_init(map,quest_map,player);
    init_quete(&quete,quest_map);
    save(player,map,quest_map,quete,*sauv);
    printf("\n   >>> Initialisation. Sauvegarde automatique.\n"); sleep(1);
    presentation_regle_jeu();
  }

  if(creation_tab_item(Tab_Items, &nb_items_available)){
    menu_principal_jeu(player,map,quest_map,quete,*sauv,Tab_Items,nb_items_available);
  }
}


/**
 * \fn int main()
 * \brief Fonction principale du jeu
 * \return Rien
*/
int main(){
  sauv_t sauv;
  int choix;
  srand(time(NULL));

  menu:
  update_etat_sauvegarde(&sauv);
  clrscr();
  affichage_parties(sauv);
  printf("   BIENVENUE - Que souhaitez-vous faire ?\n");
  printf("   1. Choisir une partie\n");
  printf("   2. Effacer une partie\n");
  printf("   3. Quitter le jeu\n");
  printf("\n   Votre choix : ");
  do {
    scanf("%d",&choix);
    if(choix < 1 || choix > 3){
      printf("   Valeur incorrecte. Veuillez ressaisir : ");
    }
  } while (choix < 1 || choix > 3);

  if(choix == 1){
    choix_partie(&sauv);
  }
  else if(choix == 2){
    effacer_partie(sauv);
    goto menu;
  }
 clrscr();
}
