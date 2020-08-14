#include <time.h>
/*-----------------------------------------------------------*/
long int cmp2rec       (t_d_s *p_1, t_d_s *p_2);
void	 binary_finding(t_d_s *P_FND);
void     on_equal_when_binary_finding(t_d_s *p_equ);
void     create_new_rec();
void     form_new_rec();
void     insert_new_rec();
void     fix_first_rec();

/*-----------------------------------------------------------*/
void alloc_string_incl2() {

  /*   */
  if (!n_rec) {
    create_new_rec();
    fix_first_rec();
  }
  else {
    binary_finding(&P_FrA);     // return fs & P_BFL as GLOBAL 
    if (fs) {
      create_new_rec();		// Create new record (p_new)
      insert_new_rec();		// Insert new record (p_new) относительно P_BFL
    }
  }
}
/*-----------------------------------------------------------*/
void alloc_string_incl_naoborot() {
  /*   */

  if (!n_rec) {
    create_new_rec();
    fix_first_rec();
  }
  else {
    binary_finding(&P_FrA);     // return fs & P_BFL as GLOBAL 
    if (!fs) {                  // Отличие - ТУТ !!!
      create_new_rec();		// Create new record (p_new)
      insert_new_rec();		// Insert new record (p_new) относительно P_BFL
    }
  }
}
/*-----------------------------------------------------------*/
unsigned int bf_n_extreme     	= 0;		// Кол-во binary_finding в режиме extreme
unsigned int c2r_extreme_c  	= 0;		// Кол-во вызовов  cmp2rec в режиме extreme
unsigned int c2r_extreme_l	= 0;		// Сработал левый  extreme
unsigned int c2r_extreme_r	= 0;		// Сработал правый extreme
unsigned int c2r_extreme_e	= 0;		// В режиме extreme попали на равенство

unsigned int bf_n_bintree     	= 0;		// Кол-во binary_finding в режиме bintree
unsigned int c2r_bintree_c  	= 0;		// Кол-во вызовов  cmp2rec в бинарном дереве
unsigned int c2r_bintree_l	= 0;		// Сделан левый  шаг по бинарному дереву
unsigned int c2r_bintree_r	= 0;		// Сделан правый шаг по бинарному дереву
unsigned int c2r_bintree_e	= 0;		// В конце концов попали на равенство
/*-----------------------------------------------------------*/
void binary_finding(t_d_s *p_fnd) {

int   f_cyc;
long  fs_loc; 
t_d_s *p_tmp;

/*		Эта проверка должна происходить выше.
  if (!n_rec) {
    return NULL;
  }
*/

// Если предыдущий прирост был в самый конец  ДМ, и текущий прирост - тоже в самый конец  ДМ, то
// надо сразу вставлять новую запись в конец  ДМ 

//  f_cyc = 0 - это означает, что искомое где-то в середине ДМ

//  Чаще бывает, что массив отсортирован по ВОЗРАСТАНИЮ, и новую запись надо помещать в конец. 
//  Поэтому первым делом проверяем правый угол пирамиды.

//  fl_extreme = 0;


  if (fl_extreme) {    
    // В предыдущий раз новая запись встала в самый край.

    bf_n_extreme++;

    p_ii2  = p_x_hi2;
    p_ii1  = p_x_hi1;
    p_ii0  = p_x_hi0;

    c2r_extreme_c++; // Статистика
    fs = cmp2rec(p_fnd, p_ii0);                // Сравнение нового элемента в предыдущим, который был расположен с краю

    if (!fs) {                                 // Если fs равно нулю, то on_equal
      on_equal_when_binary_finding(p_ii0);     // Не забываем, что в on_equal fs может поменяться на -1 или +1 
      c2r_extreme_e++;
    }
    else 
    if ((fs ^ fl_extreme) < 0) {               // Если fs противоположно по знаку fl_extreme
      fl_extreme = 0;                          //   то будем искать с середины
    }                                          
    else { // Статистика
      if (fl_extreme > 0)
        c2r_extreme_r++;
      else
      if (fl_extreme < 0)
        c2r_extreme_l++;
    }
  }                                            

  if (!fl_extreme) {                           // т.е. if (fl_extreme == 0)
    bf_n_bintree++;
    p_ii0  = p_mid;
    p_ii1  = NULL;
    p_ii2  = NULL;
    fs     = 0;
    
    while (1) {

      n_c3++;  // Не комментарить, этот счетчик нужен для Разбивки

      fs_1  = fs;                  // Сохраняем историю сравнений

      c2r_bintree_c++;             // Статистика
      fs = cmp2rec(p_fnd, p_ii0);  // Сравниваем.

// У binary_finding одна цель: НАЙТИ
// Соответственно ны выходе должно быть либо Указатель на найденный элемент либо NULL

// Вариант действий при полном равенстве:
// 1. Разместить
// 2. Не размещать, просуммировать
// 3. Не размещать, ничего не делать

// Выбор варианта был в on_full_equal
// В большинстве случаев on_full_equal - пустой.
// Раньше это приводило к варианту №1.
// Сейчас, чтобы выйти на вариант №1, 
//   надо явно в on_full_equal прописать fs = 1 
//   (если вставка нового элемента справа от равнозначного)
//   или fs = -1 
//   (если вставка нового элемента слева)

// Чтобы в других программах не запутаться, заменим вызов on_full_equal на on_equal

// При варианте 1 нельзя делать BREAK, надо дойти до подножия пирамиды  !!!
// При варианте 2 надо сделать  BREAK, до подножия пирамиды идти не надо!!!
// При варианте 3 надо сделать  BREAK, до подножия пирамиды идти не надо!!!
      if (!fs) {
        on_equal_when_binary_finding(p_ii0);
// в on_equal() fs должно измениться:
// при вставке справа - fs = 1, слева - -1.
// при суммировании или поиске - fs не меняется (остается равным нулю).
      }

      fs_loc = fs;       // fs_loc хранится на регистре!!!
      if (fs_loc == 0) {
        c2r_bintree_e++; // Статистика
        break;           // Выход из цикла при полном равенстве в режиме суммирования или поиска
      }
      else 
      if (fs_loc < 0) {
        c2r_bintree_l++; // Статистика
        p_tmp = p_ii0->p_poi[2];
        if (!p_tmp) {    // Выход из цикла когда дошли до подножия пирамиды                  
          v_v = 2;
          break;
        }
      }
      else
      if (fs_loc > 0) {
        c2r_bintree_r++; // Статистика
        p_tmp = p_ii0->p_poi[3];
        if (!p_tmp) {    // Выход из цикла когда дошли до подножия пирамиды                  
          v_v = 3;
          break;
        }
      }
    
      p_ii2  = p_ii1;	// Сдвигаем историю дерева
      p_ii1  = p_ii0;	// Сдвигаем историю дерева
      p_ii0  = p_tmp;	// Сдвигаем историю дерева

    }
  }

  P_BFL = p_ii0;	// возвращаем указатель на точное совпадение либо на последний сравненный элемент

}

/*-----------------------------------------------------------*/
void create_new_rec() {

  p_new = (t_d_s *) calloc(1, sizeof(t_d_s));
  if (!p_new) {
    printf("‡ Є®­зЁ« бм ¤Ё­ ¬ЁзҐбЄ п Ї ¬пвм. ЉапЄ-ЄапЄ...\n");
    exit(1);
  }

  form_new_rec();
  p_new->p_poi[0] = NULL;
  p_new->p_poi[1] = NULL;
  p_new->p_poi[2] = NULL;
  p_new->p_poi[3] = NULL;
  n_rec++;

  return;
}

/*-----------------------------------------------------------*/
void fix_first_rec() {

  fl_extreme = 0;
  n_c3 = 0;

  p_beg = p_new;
  p_fin = p_new;
  p_mid = p_new;
  p_new->p_poi[0] = NULL;
  p_new->p_poi[1] = NULL;
  return;
}
/*-----------------------------------------------------------*/
void insert_new_rec() {
// Вставка записи NEW относительно P_BFL
// Меняются только горизонтальные указатели

t_d_s *p_l;         
t_d_s *p_r;         
t_d_s *p_c;
t_d_s *p_prev;
t_d_s *p_x;
t_d_s *p_kkk;
int   i;
  
  p_kkk = P_BFL;
/*	Вроде как излишняя проверка
  if (!n_rec) {
    return;
  }
*/
/**********************************************************************/
/* Прописка указателей на нижнем уровне */

  p_c = p_new;

  if (fs > 0) {
    p_l = p_kkk; p_r = p_l->p_poi[1]; // Вставка будет справа от p_kkk
    if (p_r) 
      p_r->p_poi[0] = p_c;            // Вставка попала в серединку
    else {
      p_fin = p_c; p_c->p_poi[1] = NULL; p_x_hi0 = p_c; p_x_hi1  = p_kkk; p_x_hi2 = p_ii1; fl_extreme =  1; 
    }
    p_l->p_poi[1] = p_c;              // Вставка попала в серединку
  }
  // fs == 0 - такого тут быть не должно
  else {
    p_r = p_kkk; p_l = p_r->p_poi[0]; // Вставка будет слева от p_kkk    
    if (p_l) 
      p_l->p_poi[1] = p_c;            // Вставка попала в серединку
    else {
      p_beg = p_c; p_c->p_poi[0] = NULL; p_x_hi0 = p_c; p_x_hi1 = p_kkk;  p_x_hi2 = p_ii1; fl_extreme = -1; 
    }
    p_r->p_poi[0] = p_c;              // Вставка попала в серединку
  }
  p_c->p_poi[0] = p_l;  p_c->p_poi[1] = p_r;

//old
/* Анализ необходимости сделать перевертыш */
  if ((fs ^ fs_1) < 0)                        // Направления были разнознаковые
    p_x = p_kkk; // p_c == p_new;
  else 
    p_x = p_c, p_c = p_kkk; 

  p_prev = p_ii1;
//  p_c = NULL;
  if (n_rec >= 8){                            // Верхушку пирамиды - не переворачиваем
    if (!p_prev->p_poi[3]) {                      
      p_r = p_prev; p_l = p_x;                // Не было левого свеса
      // Здесь p_c не нулевой
    }
    else {
      if (!p_prev->p_poi[2]) {                      
        p_l = p_prev; p_r = p_x;                // Не было правого свеса
        // Здесь p_c не нулевой
      }
      else {
        p_c = NULL;
      }
    }
  }
  else {
    p_c = NULL;
  }

  if (p_c) {                            // Если p_c не нулевой - подвеска с перевертышем
    if (p_ii2->p_poi[2] == p_prev) {
      p_ii2->p_poi[2] = p_c;
    }
    else 
    if (p_ii2->p_poi[3] == p_prev) {
      p_ii2->p_poi[3] = p_c;
    }

    p_c->p_poi[2] = p_l;     p_c->p_poi[3] = p_r;
    p_l->p_poi[2] = NULL;    p_l->p_poi[3] = NULL;
    p_r->p_poi[2] = NULL;    p_r->p_poi[3] = NULL;
  }
  else {                               // Иначе подвеска без перевертыша
    p_kkk->p_poi[v_v] = p_new;
  }
  
  /* Разбивка */
  /* Делается если кол-во записей достигло очередного уровня 2**n */  
  /* или количество обычных сравнений в текущем цикле, возведенное в квадрат, превысило кол-во записей) */

/*
/*  if ((n_rec >= v_2n - 1) || (n_c*2  > n_c2 * n_c2)) {*/

  if (n_rec >= v_2n - 1) {
    n_rz++;
    p_mid = bin_razbiv2(p_beg, p_fin);
    n_c3 = 0;
  }
  else 
  if (n_c3 > v_n * v_2n) {
    n_rz++;
    p_mid = bin_razbiv2(p_beg, p_fin);
    n_c3 = 0;
  }

  while (n_rec >= v_2n - 1) {
    v_2n = v_2n + v_2n;		// v_2n     ЇаЁ­Ё¬ Ґв §­ зҐ­Ёп 1, 2, 4, 8, 16, ....., 65536, ... 
    v_n++;                      // v_n      ЇаЁ­Ё¬ Ґв §­ зҐ­Ёп 0, 1, 2, 3,  4, .....,    10, ... 
  }
  return;

}
/******************************************************************/
int sort_bin(t_d_s *p_bbb, t_d_s *p_fff) {
int i;

t_d_s *p_p;
t_d_s *p_next;

  fl_extreme = 0;

  n_rec  = 0;
  v_2n   = 1;
  v_n    = 0;

  p_bbb->p_poi[0] == NULL;
  p_fff->p_poi[1] == NULL;
  p_beg = NULL; 
  p_fin = NULL;

  for (p_new = p_bbb; p_new != NULL; p_new = p_next) {
     p_next = p_new->p_poi[1];

    if (!n_rec) {
      fix_first_rec();  // Здесь глобальные p_beg и p_fin инициализируются заново.
    }
    else {
      binary_finding(p_new);   /* return fs & P_BFL as GLOBAL */
      insert_new_rec();                                        
    }
    n_rec++;
  }

//  p_mid = bin_razbiv2(p_beg, p_fin); // Разбивка идет по глобальным p_beg и p_fin

  return 0;
}

/* ************************************************************* */
void del_rec(t_d_s *p_del) {
// Логическое удаление записи p_del.
// Меняются только горизонтальные указатели.
// Указатели соседних записей будут указывать не на удаляемую запись, а друг на друга.

// Физически p_del остается не месте без изменений, 
// указатели P_poi, по-прежнему указывают на соседние записи.
// Так что если мы вызываем del_rec из цикла, 
// то в этом цикле мы перейдем на следующую соседнюю запись.

t_d_s *p_lft;
t_d_s *p_rgt;

  p_lft = p_del->p_poi[0];   /* Теперь p_lft указывает на предыдущее звено */
  p_rgt = p_del->p_poi[1];   /* Теперь p_rfg указывает на следующее  звено */

  n_rec--;
  if ((p_del != p_beg) && (p_del != p_fin)) {
    p_lft->p_poi[1] = p_rgt; /* Правый ук. с предыдущего зв. будет указывать на следующее  зв. */
    p_rgt->p_poi[0] = p_lft; /* Левый  ук. со следующего зв. будет указывать на предыдущее зв. */
    return;
  }

  if ((p_del == p_beg) && (p_del != p_fin)) {
    p_beg = p_rgt;
    p_beg->p_poi[0] = NULL;
    return;
  }

  if ((p_del != p_beg) && (p_del == p_fin)) {
    p_fin = p_lft;
    p_fin->p_poi[1] = NULL;
    return;
  }

  if ((p_del == p_beg) && (p_del == p_fin)) {
    p_beg = NULL; 
    p_fin = NULL; 
    return;
  }
  return;
}

/*-----------------------------------------------------------*/
void alloc_string_nosort() {
  p_new = (t_d_s *) calloc(1, sizeof(t_d_s));
  if (p_new == NULL) {
    printf("Закончилась динамическая память. Кряк-кряк...\n");
    exit(1);
  }
  form_new_rec();

  n_rec++;

  if (n_rec == 1) {
    p_beg = p_new;
    p_beg->p_poi[0] = NULL;
  }
  else {
    // p_fin сейчас предыдущая запись, а p_new - текущая
    p_fin->p_poi[1] = p_new;
    p_new->p_poi[0] = p_fin;
  }
  p_fin = p_new;
  p_fin->p_poi[1] = NULL;
}
