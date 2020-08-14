#include <alloc.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys\stat.h>

#include <my_string.hpp>

FILE     *in;

const unsigned long int shag_trass = 100000;

const unsigned long int mlrd   = 1000000000;
const unsigned      int d_mlrd = 9;

/* Набор ключей в цифровом варианте */
unsigned int fl_g    = 0;
unsigned int fl_c    = 0;
unsigned int fl_b    = 0;
unsigned int fl_e    = 0;
unsigned int fl_v    = 0;
unsigned int fl_s    = 0;
unsigned int fl_i    = 0;
unsigned int fl_n    = 0;
unsigned int fl__    = 0;
unsigned int fl_fn   = 0;
unsigned int fl_si   = 0;
unsigned int fl_ns   = 0;
unsigned int fl_yo   = 0;
unsigned int fl_cou  = 0;
unsigned int fl_fir  = 0;
unsigned int fl_exa  = 0;
unsigned int fl_npos = 0;
unsigned int fl_sam  = 0;
unsigned int fl_nstr = 0;

int fl_sum;

int return_when_found;
int return_when_not_found;

/* Набор ключей в цифровом варианте */

long              nom_beg;
int               min_dl = 32000;
int               min_dl1;            // Принимает значение на единичку меньше чем min_dl

//unsigned long int n_compare;
//unsigned long int n_compare2;
//         long int n_compare_desc;

char             *str_find;
struct_str_spec  string_pnt;

char     *dop_switch;

char     Sn_read_all[100];
char     Sn_find_all[100];

char     file_litle    [2000];
char     file_great    [2000];
char     file_litle_rus[2000];
char     file_great_rus[2000];
char     str_litle     [2000];

char     swch0[1000];
char     swch [1000];
char     swch3[1000];

char     syms_for_parse[100];

char              *c_sep;
char              *c_col;

int               rez;
int               num_col;

int               npos1 = 0;    // По умолчанию - единица
int               npos2 = 0;    // npos2 - число не абсолютное, а относительное относительно npos1 !

long int          nstr1 = 1;    // По умолчанию - единица
long int          nstr2 = 0;    // nstr2 - число не абсолютное, а относительное относительно nstr1 !
long int          nstr0;
                  
unsigned long 	  nf;

unsigned char	  sym1;
unsigned char	  sym1_min = 255;
unsigned char	  sym1_max = 0;

unsigned long int n2_str_desc;
unsigned long int n2_str;

unsigned long int n_read;      // Кол-во считанных строк в одном файле
unsigned long int n_find;      // Кол-во найденных строк в одном файле

unsigned long int n_read_all;  // Кол-во считанных строк во всех файлах / до миллиарда
unsigned long int n_find_all;  // Кол-во найденных строк во всех файлах / до миллиарда

unsigned long int n_read_all2; // Счетчик миллиардов
unsigned long int n_find_all2; // Счетчик миллиардов

int               nn;

char     *p;

struct t_d_s {
       char  		*ss_str;
       unsigned int     ll_str ;
       t_d_s 		*p_poi[4];
};

t_d_s  P_FrA;

unsigned int  i, j;
int k;

#include <scan1level.hpp>
#include <t_d_s.hpp>
#include <alloc_with_bin_sort.hpp>
#include <sort_merge.hpp>

/*-----------------------------------------------------------*/
void form_new_rec() {
  p_new->ss_str = din_alloc(P_FrA.ss_str);
  p_new->ll_str = P_FrA.ll_str;
}

/*-----------------------------------------------------------*/
// Это заглушка. Сравнение происходит в find_0.


long int cmp2rec(t_d_s *p_1, t_d_s *p_2) {

long int fs;

// В данном случае p_1 - это НОВАЯ строка, а p_2 - строка, которая уже в динмассиве.

// Как показали эксперименты, отдельная проверка первого символа только замедляет работу на 10%
//  fs = (unsigned char) p_1->ss_str[0] - (unsigned char) p_2->ss_str[0];  if (fs) return fs;

//printf("1.%s\n",  p_1->ss_str);
//printf("2.%s\n",  p_2->ss_str);
//printf("3.%3d\n", p_2->ll_str);

//fs = strncmp (p_1->ss_str, p_2->ss_str, p_2->ll_str);
//printf("4.%3d\n", fs);

  // p_1 - проверяемая строка
  // p_2 - образец

  fs = strncmp (p_1->ss_str, p_2->ss_str, p_2->ll_str);
  return fs;

}
/*-----------------------------------------------------------*/
void on_equal_when_binary_finding(t_d_s *p_equ) {
  // При нахождении бинарный поиск прекращается.
}
/*-----------------------------------------------------------*/
int save_in_mem(char *str_x) {

  if (fl__) {
    parse_string(str_x, syms_for_parse);
    str_x = parts[num_col - 1];
  }

  l_s = strlen(str_x);
  if (l_s == 0) {
    return 0;
  }

  if (str_x[l_s-1] == '\n') {
    l_s--;
    if (l_s == 0) {
      return 0;
    }
    str_x[l_s] = '\0';
  }

  if (fl_e) {
    strrev(str_x);
  }
  if (fl_i) {
    to_upper(str_x);        // собственная функция strupr с русскими буквами не работает
  }
  if (fl_ns) {
    delete_space(str_x);
    l_s = strlen(str_x);
    if (l_s == 0) {
      return 1;
    }
  }
  if (fl_yo) {
    repl_yo(str_x);
  }

  min_dl = min(min_dl, l_s);

  sym1 = (unsigned char) str_x[0];

  sym1_min = min(sym1, sym1_min);
  sym1_max = max(sym1, sym1_max);

  P_FrA.ss_str = str_x;
  P_FrA.ll_str = strlen(str_x);

  alloc_string_nosort();
  return 1;
}

/*-----------------------------------------------------------*/
void print_rez() {
char stri2[1000];

  if (fl_cou == 0) {

    if (fl_fn) {
      sprintf(stri2, "%s;", fullfilename);
//      printf("%s", stri2);
      print_to_buff(stri2, "");
    }

    if (fl_n) {
      sprintf(stri2, "%ld;", n_read + nom_beg + nstr1 - 1);
      print_to_buff(stri2, "");
    }

    if (fl_sam) {
      sprintf(stri2, "%s;", P_BFL->ss_str);
//      printf("%s", stri2);
      print_to_buff(stri2, "");
    }

//    printf("%s\n", string_pnt);
    print_to_buff(string_pnt.str, "\n");
  }

  n_find++;
  n_find_all++;
  if (n_find_all == mlrd) {
    n_find_all = 0;
    n_find_all2++;
  }
}
/*-----------------------------------------------------------*/
int find_1() {

int            max_cmp;
unsigned char  sym_first;

long           fs_loc; 
t_d_s          *p_tmp;
char           *str_to_find;

char           *s1;
char           *s2;
int            i2;

  str_to_find = str_find;
  if (fl_b)                         // При ключе /B - только одно сравнение с началом
                                    // fl_b также равен единице, если есть ключи /E или /EXACT
    max_cmp = 1;
  else
    max_cmp = strlen(str_to_find) - min_dl1;

  p_tmp = p_mid;

  for (; max_cmp > 0; str_to_find++, max_cmp--) {
    for (i2 = p_tmp->ll_str, s1 = str_to_find, s2 = p_tmp->ss_str; i2 != 0; i2--, s1++, s2++) {
      fs_loc = (unsigned char) *s1 - (unsigned char) *s2;
      if (fs_loc) {
        break;
      }
    }

    if (!fs_loc) {               // Если скользящий указатель не NULL, значит нашлось совпадение
      P_BFL = p_tmp;
      if (!fl_exa) {
        return return_when_found;
      }
      else {
        if (strlen(str_find) == p_mid->ll_str) {
          return return_when_found;
        }
      }
    }
  }

  return return_when_not_found;
}
/*-----------------------------------------------------------*/
int find_0() {

int            max_cmp;
unsigned char  sym_first;

long           fs_loc; 
t_d_s          *p_tmp;
char           *str_to_find;

char           *s1;
char           *s2;
int            i2;

  str_to_find = str_find;
  if (fl_b)                         // При ключе /B - только одно сравнение с началом
                                    // fl_b также равен единице, если есть ключи /E или /EXACT
    max_cmp = 1;
  else
    max_cmp = strlen(str_to_find) - min_dl1;

  for (; max_cmp > 0; str_to_find++, max_cmp--) {
    sym_first = (unsigned char) *str_to_find;
    if ((sym_first >= sym1_min) && (sym_first <= sym1_max)) {

      for (p_tmp = p_mid; p_tmp != NULL; ) {      // Выход из цикла когда дошли до подножия пирамиды                  


//        fs_loc = strncmp (str_to_find, p_tmp->ss_str, p_tmp->ll_str);

// Посмотрено via assembler - все оптимально дальше некуда, 
// самое главное, что s1 и s2 - хранятся на регистрах

        for (i2 = p_tmp->ll_str, s1 = str_to_find, s2 = p_tmp->ss_str; i2 != 0; i2--, s1++, s2++) {
          fs_loc = (unsigned char) *s1 - (unsigned char) *s2;
          if (fs_loc < 0) {
            p_tmp = p_tmp->p_poi[2];
            break;
          }
          else
          if (fs_loc > 0) {
            p_tmp = p_tmp->p_poi[3];
            break;
          }
        }
        if (fs_loc == 0) {       // fs_loc == 0, - это полное совпадение
          break;
        }
      }

      if (p_tmp) {               // Если скользящий указатель не NULL, значит нашлось совпадение
        P_BFL = p_tmp;
        if (!fl_exa) {
          return return_when_found;
        }
        else {
          if (strlen(str_find) == p_tmp->ll_str) {
            return return_when_found;
          }
        }
      }
    }
  }

  return return_when_not_found;
}
/*-----------------------------------------------------------*/
void del_dubl_1() {

t_d_s *p1, *p2;
int   ll;

  for (p1 = p_beg;;) {
    p2 = p1->p_poi[1];
    if (p2 == NULL) {
      break;
    }

    /*
    Массив отсортирован.

    Вот у нас две строки:

        abcdef
        abcdefghjytfdfd

    Если ключ EXACT не задан, то сравнение будет производиться по длине первой короткой строки.
      Будет получен положительный результат и вторая строка будет удалена.

    Если же ключ EXACT задан, то сравнение будет производиться по длине второй длинной строки.
      Будет получен отрицательный результат и вторая строка удалена не будет.
    */

    if (fl_exa == 0)
      ll = p1->ll_str;
    else
      ll = p2->ll_str;

    if (strncmp(p1->ss_str, p2->ss_str, ll) == 0) {
      del_rec(p2);
    }
    else {
      p1 = p2; 
    }
  }
}
/*------------------------------------------------*/
void del_dubl_1_exa() {

t_d_s *p1, *p2;

  for (p1 = p_beg;;) {
    p2 = p1->p_poi[1];
    if (p2 == NULL) {
      break;
    }

    if (strcmp(p1->ss_str, p2->ss_str) == 0) {
      del_rec(p2);
    }
    else {
      p1 = p2; 
    }
  }
}
/*-----------------------------------------------------------*/
void del_dubl_2() {

t_d_s *p1, *p2;

  if ((fl_b == 0) && (fl_e == 0) && (fl_exa == 0)) {
    p_mid = bin_razbiv2(p_beg, p_fin);

    return_when_found = 1;

    for (p1 = p_beg; p1 != NULL; p1 = p1->p_poi[1]) {
      str_find = p1->ss_str + 1;                               // str_find - глобальный параметр для find_0()

      // Корректно ли удалять элемент, когда в последующем будет еще неоднократное binary_finding?
      // Ведь при del_rec ссылки двоичного дерева никак не меняются.
      // Не лучше ли пометить элемент к удалению, а потом отдельным циклом удалить?

      // Во первых, помечать нечем. Чтобы помечать, надо вводить дополнительную ячейку.
      // Во-вторых, при del_rec() дерево остается прежним, 
      //   это только для "цепочки на столе" одно звено пропадает.

      if (find_0()) {
        del_rec(p1);
      }
    }
  }
}

/*-----------------------------------------------------------*/
void trassirovka() {

char s_tmp[1000];

  if (fl_si) 
    return;

  if (n_read_all2 == 0) {                           // ‘зҐвзЁЄ ¬Ё««Ё а¤®ў
    sprintf(Sn_read_all, "%lu", n_read_all);
  }
  else {
    sprintf(s_tmp, "000000000000000000%lu", n_read_all);
    strcpy(s_tmp, s_tmp + strlen(s_tmp) - d_mlrd);
    sprintf(Sn_read_all, "%lu%s", n_read_all2, s_tmp);
  }

  if (n_find_all2 == 0) {
    sprintf(Sn_find_all, "%lu", n_find_all);
  }
  else {
    sprintf(Sn_read_all, "%lu%lu", n_find_all2, n_find_all);
  }
  cprintf("\r‘зЁв ­®/­ ©¤Ґ­® бва®Є: %15lu/%-15lu %15s/%-15s\r", n_read, n_find, Sn_read_all, Sn_find_all);

// n_read - бзҐвзЁЄ бзЁв ­­ле бва®Є ў ®¤­®¬ д ©«Ґ
// n_find - бзҐвзЁЄ ­ ©¤Ґ­­ле бва®Є ў ®¤­®¬ д ©«Ґ

// Sn_read_all - бзҐвзЁЄ бзЁв ­­ле бва®Є ў® ўбҐе д ©« е
// Sn_find_all - бзҐвзЁЄ ­ ©¤Ґ­­ле бва®Є ў® ўбҐе д ©« е

  return;
}
/*-----------------------------------------------------------*/
/* оригинальная обработка открытого файла */
/* Два варианта */

void original_works_binary_file1();  /* оригинальная обработка открытого файла */
void original_works_binary_file2();  /* оригинальная обработка открытого файла */
/*-----------------------------------------------------------*/

void original_works_binary_file (struct ffblk *ffblk_current) {

  switch (f_ow) {
  case 1: original_works_binary_file1(); return;
  case 2: original_works_binary_file2(); return;
  }
  return;
}


/*-----------------------------------------------------------*/
/* оригинальная обработка открытого файла */
/* Два варианта */

void original_works1 ();
void original_works2 ();
/*-----------------------------------------------------------*/

void original_works (struct ffblk *ffblk_current) {

  switch (f_ow) {
  case 1: original_works1(); return;
  case 2: original_works2(); return;
  }
  return;
}

/* -----------------------------------------------------------*/

/* Оригинальная обработка открытого файла. */

void original_works1 () {
  int i, j;

  if (!fl_si) cprintf("\r%s\r\n", fullfilename_rus);

  n2_str = 0;
  n2_str_desc = shag_trass;

  while (1) {

    *string = '\0';
    fgets(string,MXLEN,in);

    if (feof(in)) {
      if (*string == '\0') {
        break;
      }
    }

    if (save_in_mem(string)) {
      n_str++;
      n2_str++;
      n2_str_desc--;
    }

    if (n2_str_desc == 0) {
      if (!fl_si) cprintf("\r‘зЁв ­® бва®Є:                  %10ld\r", n2_str);
      n2_str_desc = shag_trass;
    }
  }

  if (!fl_si) cprintf("\r‘зЁв ­® бва®Є:                  %10ld\r\n", n2_str);
}

/*-------------------------------------------------*/
void original_works2 () {
int i;
int lx;
int llx;

  if (!fl_si) cprintf("\r\n%s\r\n", fullfilename_rus);

  n_str  = 0;
  n_read = 0;
  n2_str_desc = shag_trass;
  n_find = 0;

  while (1) {

    if (fl_fir)
      if (n_find == fl_fir)
        break;

    *string = '\0';
    fgets(string, MXLEN, in);

    if (feof(in))
      break;

    n_str++;

    if (n2_str_desc == 0) {
      trassirovka();
      n2_str_desc = shag_trass;
    }
    n2_str_desc--;

    if (n_str < nstr1) {
      continue;
    }
    if (nstr2 > 0) {
      if (n_str >= nstr1 + nstr2) {
        break;
    }}

    n_read++;
    n_read_all++;
    if (n_read_all == mlrd) {
      n_read_all = 0;
      n_read_all2++;
    }

    // Последним символом в прочитанной строке должен быть символ /n.
    // А если не /n - значит в прочитанной строке есть терминатор(ы) - '\0'
    // Надо их превратить в пробелы.

    lx = strlen(string) - 1;               // Номер последнего значащего символа в строке

    if (string[lx] != '\n') {
      for (i = lx; string[i] != '\n'; i++) {
        if (string[i] == '\0') {           // В прочитанной строке есть терминаторы
          string[i] = ' ';
        }
      }
      lx = strlen(string) - 1;               // Последний символ - \n
    }

    string[lx] = '\0';
    if (lx < min_dl) {
      if (fl_v) {                          // Инверсный режим
        print_rez();
      }
      continue;
    }

    str_find       = string;
    string_pnt.str = string;		// С этого указателя печатаем строку

    // При некоторых ключах исходная строка претерпевает одно или несколько изменений:
    //    переворачивается наоборот
    //    переводится в верхний регистр
    //    Ё -> Е
    //    удаляются все пробелы
    //    из строки делается вырезка с позиции 1 по позицию 2
    //
    //    Поэтому:
    //      string   - исходная строка
    //      string2  - возможно, преобразованная строка
    //      str_find - указатель на string или string2 (если строка преобразовывалась)

    if (fl_sum) {
      strcpy(string2, string);
      str_find = string2;


      if (fl_npos) {
        //lx = strlen(str_find);

        if (npos1 > 0) {
          if (npos1 < lx) {
            str_find = str_find + npos1 - 1;
          }
          else {
            *str_find = '\0';
          }
        }
        else {
          if (-npos1 < lx) {
            str_find = str_find + npos1 + lx;	
          }
          else {
            *str_find = '\0';
          }
        }
        if (npos2 > 0)
          if (npos2 < MXLEN)
            str_find[npos2] = '\0';
      }

      if (fl_ns)  {                        	// Удаляем все пробелы
        delete_space(str_find);

        lx = strlen(str_find);
        if (lx < min_dl) {
          if (fl_v) {                      // Инверсный режим
            print_rez();
          }
          continue;
        }
      }

      if (fl_e ) strrev   (str_find);    // Переворачиваем
      if (fl_i ) to_upper (str_find);    // В верхний регистр
      if (fl_yo) repl_yo  (str_find);    // Заменяем Ё на Е
    }

    // Ищем что-то в str_find

    if (*str_find != '\0') {
      if (find_0()) {                    // Реально нашли!!!
        print_rez();
      }
    }
  }

  // Файл закончился
  trassirovka();                         // Окончательные цифры по одном файлу

  if (!fl_si) cprintf("\n");
  if (fl_cou != 0) {
    printf("Ќ ©¤Ґ­® бва®Є: %15ld. ” ©«: %s\n", n_find, fullfilename_rus);
  }
}
/* -----------------------------------------------------------*/

/* Оригинальная обработка открытого файла. */

void original_works_binary_file1 () {
  int i, j;

  if (!fl_si) cprintf("\r%s\r\n", fullfilename_rus);

  n2_str = 0;
  n2_str_desc = shag_trass;

  while (1) {

    string_pnt = fgets_from_bin_file(IN2);
    if (string_pnt.str == NULL)
      break;

    if (save_in_mem(string_pnt.str)) {
      n_str++;
      n2_str++;
      n2_str_desc--;
    }

    if (n2_str_desc == 0) {
      if (!fl_si) cprintf("\r‘зЁв ­® бва®Є:                  %10ld\r", n2_str);
      n2_str_desc = shag_trass;
    }
  }

  if (!fl_si) cprintf("\r‘зЁв ­® бва®Є:                  %10ld\r\n", n2_str);
}

/*-------------------------------------------------*/
/* Оригинальная обработка открытого файла.       */

void original_works_binary_file2() { /* оригинальная обработка открытого файла */

int i;
int lx;
int llx;

  if (!fl_si) cprintf("\r\n%s\r\n", fullfilename_rus);

  n_str  = 0;
  n_read = 0;
  n2_str_desc = shag_trass;
  n_find = 0;

  while (1) {

    if (fl_fir)
      if (n_find == fl_fir)
        break;

    string_pnt = fgets_from_bin_file(IN2);
    if (string_pnt.str == NULL)
      break;

    n_str++;

    if (n2_str_desc == 0) {
      trassirovka();
      n2_str_desc = shag_trass;
    }
    n2_str_desc--;

    if (n_str < nstr1) {
      continue;
    }
    if (nstr2 > 0) {
      if (n_str >= nstr1 + nstr2) {
        break;
    }}

    n_read++;
    n_read_all++;
    if (n_read_all == mlrd) {
      n_read_all = 0;
      n_read_all2++;
    }

    // Если вычисленная длина строки меньше длины строки, переданной из функции fgets_from_bin_file,
    // значит в середине строки есть терминатор(ы) - '\0'
    // Надо их превратить в пробелы.

    for (i = strlen(string_pnt.str); i < string_pnt.len; i++) {
      if (string_pnt.str[i] == '\0') {           // В прочитанной строке есть терминаторы
        string_pnt.str[i] = ' ';
      }
    }

    if (string_pnt.len < min_dl) {
      if (fl_v) {                              // Инверсный режим
        print_rez();
      }
      continue;
    }

//printf("%s\n", string_pnt);
//continue;

    str_find = string_pnt.str;
    // При некоторых ключах исходная строка претерпевает одно или несколько изменений:
    //    переворачивается наоборот
    //    переводится в верхний регистр
    //    Ё -> Е
    //    удаляются все пробелы
    //    из строки делается вырезка с позиции 1 по позицию 2
    //
    //    Поэтому:
    //      string   - исходная строка
    //      string2  - возможно, преобразованная строка
    //      str_find - указатель на string или string2 (если строка преобразовывалась)

    if (fl_sum) {
      strcpy(string2, string_pnt.str);
      str_find = string2;


      if (fl_npos) {
        //lx = strlen(str_find);

        if (npos1 > 0) {
          if (npos1 < lx) {
            str_find = str_find + npos1 - 1;
          }
          else {
            *str_find = '\0';
          }
        }
        else {
          if (-npos1 < lx) {
            str_find = str_find + npos1 + lx;	
          }
          else {
            *str_find = '\0';
          }
        }
        if (npos2 > 0)
          if (npos2 < MXLEN)
            str_find[npos2] = '\0';
      }

      if (fl_ns)  {                        	// Удаляем все пробелы
        delete_space(str_find);

        lx = strlen(str_find);
        if (lx < min_dl) {
          if (fl_v) {                      // Инверсный режим
            print_rez();
          }
          continue;
        }
      }

      if (fl_e ) strrev   (str_find);    // Переворачиваем
      if (fl_i ) to_upper (str_find);    // В верхний регистр
      if (fl_yo) repl_yo  (str_find);    // Заменяем Ё на Е
    }

    // Ищем что-то в str_find

    if (*str_find != '\0') {
      if (n_rec == 1) {
        if (find_1()) {                    // Реально нашли!!!
          print_rez();
        }
      }
      else {
        if (find_0()) {                    // Реально нашли!!!
          print_rez();
        }
      }
    }
  }

  // Файл закончился
  trassirovka();                         // Окончательные цифры по одном файлу

  if (!fl_si) cprintf("\n");
  if (fl_cou != 0) {
    // Вот с этим то же что-то надо сделать.
    printf("Ќ ©¤Ґ­® бва®Є: %15ld. ” ©«: %s\n", n_find, fullfilename_rus);
  }
}

/*-------------------------------------------------*/

int main(int argc, char *argv[]) {

  div_mask(argv[0], onlypath, onlymask);

  if (argc <= 1) {
    printf("Џа®Ја ¬¬  а Ў®в Ґв  ­ «®ЈЁз­® findstr, в®«мЄ® б агббЄЁ¬Ё ЎгЄў ¬Ё Ў®«ҐҐ ­ ¤Ґ¦­®.\n");
    printf("€ ў®®ЎйҐ ЎлбваҐҐ. Ќ  Ў®«миЁе ¬ ббЁў е - ®б®ЎҐ­­®. Ќ® ЎҐ§ аҐЈг«па­ле ўла ¦Ґ­Ё©.\n");
    printf("\n");
    printf("Ћб­®ў­®© ў аЁ ­в § ЇгбЄ : %s /g:” ©«-зв®-ЁбЄ вм ” ©«л-Ј¤Ґ-ЁбЄ вм <Љ«озЁ>\n", onlymask);
    printf("\n");
    printf("ђҐ «Ё§®ў ­л б«Ґ¤гойЁҐ Є«озЁ:\n");
    printf("/g:д ©«     - ‚§пвм ®Ўа §жл Ё§ § ¤ ­­®Ј® д ©« (-®ў).\n");
    printf("/c:\042бва®Є \042 - €бЇ®«м§®ў вм § ¤ ­­го бва®Єг Є Є ®Ўа §Ґж.\n");
    printf("/v          - €­ўҐаб­л© аҐ¦Ё¬ - Ї®ЁбЄ бва®Є, ­Ґ б®¤Ґа¦ йЁе б®ўЇ ¤Ґ­Ё© б ®Ўа §ж ¬Ё.\n");
    printf("/s          - Џа®е®¤ Ї® ” ©« ¬-Ј¤Ґ-ЁбЄ вм ў вҐЄгйҐ© Ї ЇЄҐ Ё ўбҐе ҐҐ Ї®¤Ї ЇЄ е.\n");
    printf("/b          - Џ®ЁбЄ ®Ўа §ж®ў в®«мЄ® ў ­ з « е бва®Є.\n");
    printf("/e          - Џ®ЁбЄ ®Ўа §ж®ў в®«мЄ® ў Є®­ж е бва®Є.\n");
    printf("/i          - Џ®ЁбЄ ЎҐ§ гзҐв  аҐЈЁбва .\n");
    printf("/npos:N1:N2 - Џ®ЁбЄ ®Ўа §ж®ў, ­ зЁ­ п б Ї®§ЁжЁЁ N1 бва®ЄЁ, ®Ў« бвм Ї®ЁбЄ  - N2 бЁ¬ў®«®ў.\n");
    printf("                (…б«Ё N1 - ®ваЁж вҐ«м­®Ґ зЁб«®, ®вбзҐв Ї®§ЁжЁЁ - б Є®­ж  бва®ЄЁ)\n");
    printf("                (…б«Ё N2 ­Ґ гЄ § ­® - Ї®ЁбЄ ўҐ¤Ґвбп ¤® Є®­ж  бва®ЄЁ)\n");
    printf("/nstr:N1:N2 - Џ®ЁбЄ ®Ўа §ж®ў, ­ зЁ­ п б® бва®ЄЁ N1, ®Ў« бвм Ї®ЁбЄ  - N2 бва®Є.\n");
    printf("                (…б«Ё N2 ­Ґ гЄ § ­® - Ї®ЁбЄ ўҐ¤Ґвбп ¤® Є®­ж  д ©« )\n");
    printf("\n");
    printf("/yo         - Џ® е®¤г ўбҐ ЎгЄўл р § ¬Ґ­повбп ­  …, ­® ­ ©¤Ґ­­лҐ бва®ЄЁ ЇҐз в овбп ў Ёбе®¤­®¬ ўЁ¤Ґ.\n");
    printf("/first      - ‚ Є ¦¤®¬ д ©«Ґ Ї®ЁбЄ ўҐ¤Ґвбп ¤® ЇҐаў®© ­ ©¤Ґ­­®© бва®ЄЁ.\n");
    printf("/first:N    - ‚ Є ¦¤®¬ д ©«Ґ Ї®ЁбЄ ўҐ¤Ґвбп ¤® ЇҐаўле N ­ ©¤Ґ­­ле бва®Є.\n");
    printf("/exact      - Џ®ЁбЄ в®з­®Ј® б®ўЇ ¤Ґ­Ёп бва®ЄЁ (жҐ«ЁЄ®¬) б ®Ўа §ж®¬.\n");

    printf("\n"); printf("Ќ ¦¬ЁвҐ Enter...\n"); gets(string);

    printf("            - ЏҐз вм ЇҐаҐ¤ Є ¦¤®© ­ ©¤Ґ­­®© бва®Є®©:\n");
    printf("/fn         --  Ё¬Ґ­Ё д ©« , ў Є®в®а®¬ ­ ©¤Ґ­  бва®Є ;\n"); 
    printf("/n          --  ­®¬Ґа  бва®ЄЁ Ёбе®¤­®Ј® д ©« ;\n");
    printf("/n:NNN      --  ­®¬Ґа  бва®ЄЁ Ёбе®¤­®Ј® д ©« , ­® ­ зЁ­ п б NNN;\n");
    printf("/prsmpl     --  б ¬®Ј® ­ ©¤Ґ­­®Ј® ®Ўа §ж  Ї®ЁбЄ ;\n");
    printf("\n");
    printf("/count             - ЏҐз вм в®«мЄ® ЉЋ‹-‚Ђ ­ ©¤Ґ­­ле бва®Є.\n");
    printf("/si                - ђҐ¦Ё¬ Silent (­  нЄа ­ ­ЁЄ ЄЁҐ б®®ЎйҐ­Ёп ­Ґ ўл¤ овбп).\n");
    printf("/ns                - ђҐ¦Ё¬ NoSpace (ў® ўбҐе бва®Є е Їа®ЎҐ«л г¤ «повбп).\n");
    printf("/out:Ё¬п-д ©«      - ‚лў®¤ аҐ§г«мв в  ў гЄ § ­­л© д ©«.\n");
    printf("/out:Ё¬п-д ©« /app - ’® ¦Ґ б ¬®Ґ б ¤®§ ЇЁбмо ў Є®­Ґж бгйҐбвўгойҐЈ® д ©« .\n");
    printf("\n");
    printf("…б«Ё ­Ё Є«оз  /g, ­Ё Є«оз  /c ­Ґ § ¤ ­®, Ї® г¬®«з ­Ёо Ўг¤Ґв ЁбЇ®«м§®ў ­ д ©« fs.lst (/g:fs.lst).\n");
    printf("…б«Ё § ¤ ­л ®Ў  нвЁе Є«оз  - Ўг¤гв ЁбЇ®«м§®ў ­л ®Ў  Ёбв®з­ЁЄ  ®Ўа §ж®ў.\n");
    printf("\n");
    printf("Љ«оз /g Ё¬ҐҐв гбЁ«Ґ­­®Ґ ¤®Ї®«­Ґ­ЁҐ ў ўЁ¤Ґ ¤®Ї®«­ЁвҐ«м­ле Ї®¤Є«озҐ© /sep Ё /col.\n");
    printf("\n");
    printf("‚ жҐ«®¬ § ЇЁбм /g:my_file.txt/sep:;!~/col:5 Ё­вҐаЇаҐвЁагҐвбп в Є:\n");
    printf("               ---           -----   -----\n");
    printf("  - ўбҐ § ЇЁбЁ д ©«  my_file.txt бҐЇ аЁаговбп бЁ¬ў®« ¬Ё <;>, <!> Ё  <~>;\n");
    printf("  - 5-п з бвм бҐЇ аЁа®ў ­­®© § ЇЁбЁ ЁбЇ®«м§гҐвбп ў Є зҐбвўҐ ®Ўа §ж  Ї®ЁбЄ .\n");
    printf("  Џа®ЎҐ«л ў­гваЁ ¤ ­­®Ј® ў аЁ ­в  ­Ґ¤®ЇгбвЁ¬л!.\n");

    printf("\n"); printf("Ќ ¦¬ЁвҐ Enter...\n"); gets(string);

    exit(1);

  }

  *file_litle = '\0';
  *file_great = '\0';
  *str_litle  = '\0';

  for (k = argc - 1 ; k > 0; k--) {
    if (*argv[k] == '/') {
      strcpy(swch,  argv[k]);
      strcpy(swch3, argv[k] + 3);

      strcpy(swch0, swch);
//    to_upper(swch);

      if (strncmpi(swch, "/G:", 3) == 0) {
        strcpy(file_litle, swch3);
        fl_g = 1;

        dop_switch = strstr(file_litle, "/");
        if (dop_switch) {
          to_upper(dop_switch);

          c_sep = strstr(dop_switch, "/SEP:");
          c_col = strstr(dop_switch, "/COL:");

          if ((c_sep) && (c_col)) {
            *c_sep = '\0';
            *c_col = '\0';
            c_sep += 5;
            c_col += 5;
            strcpy(syms_for_parse, c_sep);

            num_col = atoi(c_col);

            if (strlen(syms_for_parse) > 0) {
              if (num_col > 0) {
                fl__ = 1;
              }
            }
          }
        }
      }
      else
      if (strncmpi(swch, "/C:", 3) == 0) {		// Здесь C на латинице
        strcpy(str_litle , swch3);
        fl_c = 1;
      }
      else
      if (strncmpi(swch, "/С:", 3) == 0) {		// Здесь С на кириллице
        strcpy(str_litle , swch3);
        fl_c = 1;
      }
      else
      if (strcmpi(swch,  "/N" ) == 0) {
        fl_n = 1;
        //  На самом деле nom_beg - это смещение от естественной нумерации строк с единицы.
        nom_beg = 0;
      }
      else
      if (strncmpi(swch, "/N:", 3) == 0) {
        fl_n = 1;
        // Функция may_be_int в любом случае возвращает в переменной nom_beg
        //   результат преобразования swch3 в число.
        // При нецифровом аргументе swch3 возвращается ноль.
        // При цифровом - возвращаемое число надо уменьшить на единицу.
        if (may_be_int(swch3, nom_beg) == 1) {


printf("nom_beg = %ld\n", nom_beg);


          nom_beg--;
        }
      }
      else
      if (strcmpi(swch, "/V" ) == 0) {
        fl_v = 1;
      }
      else
      if (strcmpi(swch, "/S" ) == 0) {
        fl_s = 1;
      }
      else
      if (strcmpi(swch, "/B" ) == 0) {
        fl_b = 1;
      }
      else
      if (strcmpi(swch, "/E" ) == 0) {
        fl_e = 1;
      }
      else
      if (strcmpi(swch, "/I" ) == 0) {
        fl_i = 1;
      }
      else
      if (strcmpi(swch, "/FN") == 0) {
        fl_fn = 1;
      }
      else
      if (strcmpi(swch, "/SI") == 0) {
        fl_si = 1;
      }
      else
      if (strcmpi(swch, "/NS") == 0) {
        fl_ns = 1;
      }
      else
      if (strcmpi(swch, "/YO") == 0) {
        fl_yo = 1;
      }
      else
      if (strcmpi(swch, "/COUNT" ) == 0) {
        fl_cou = 1;
      }
      else
      if (strcmpi(swch, "/FIRST" ) == 0) {
        fl_fir = 1;
      }
      else
      if (strcmpi(swch, "/EXACT" ) == 0) {
        fl_exa = 1;
      }
      else
      if (strcmpi(swch, "/PRSMPL" ) == 0) {    // Печать образца
        fl_sam = 1;
      }
      else

      if (strncmpi(swch, "/OUT:", 5) == 0) {
        nn = parse_string(swch, "/:");
            // М.б. два варианта: 
            //                      /OUT:файл.txt            /OUT:файл.txt/APP
            // После parse будет:                            
            //                      [],[OUT],[файл.txt]      [],[OUT],[файл.txt][APP]    
            //
        OUT2 = 0;
        if (nn == 2) {
          OUT2 = open(parts[2], O_CREAT|O_TRUNC|O_RDWR|O_BINARY, S_IWRITE);
        }
        else 
        if (nn == 3) {
          if (strncmpi(parts[3], "APP", 3) == 0) {
            OUT2   = open(parts[2], O_APPEND|O_RDWR|O_BINARY, S_IWRITE);
            if (OUT2 <= 0) {
              OUT2 = open(parts[2], O_CREAT |O_RDWR|O_BINARY, S_IWRITE);
            }
          }
        }
        if (OUT2 <= 0) {
          cprintf("\rCannot open file %s\r\n", parts[1]);
          exit(1);
        }
      }
      else
      if (strncmpi(swch, "/FIRST:", 7) == 0) {
        nn = parse_string(swch, ":");
        if (nn >= 1) 
          fl_fir = atoi(parts[1]);
        else
          fl_fir = 0;
      }
      else
      if (strncmpi(swch, "/NPOS:", 6) == 0) {  // Предполагаем, что у нас ключ /NPOS:48:56
        nn = parse_string(swch, ":");
        if (nn >= 1) 
          npos1 = atoi(parts[1]);
        else
          npos1 = 0;
        if (nn >= 2) 
          npos2 = atoi(parts[2]);
        else
          npos2 = 0;

        if (npos1 != 0) {
          fl_npos = 1;
        }
        else {
          cprintf("\rWrong parameter: %s\r\n", swch0);
          exit(1);
        }
      }

      else
      if (strncmpi(swch, "/NSTR:", 6) == 0) {  // Предполагаем, что у нас ключ /NSTR:48:56

        nn = parse_string(swch, ":");

        nstr1 = 1;
        if (nn >= 1) {
          nstr0 = atol(parts[1]);
          if (nstr0 > 0) {
            nstr1 = nstr0;
          }
        }

        nstr2 = 0;
        if (nn >= 2) {
          nstr0 = atol(parts[2]);
          if (nstr0 > 0) {
            nstr2 = nstr0;
          }
        }

        if ((nstr1 > 0) && (nstr2 >= 0)){
          fl_nstr = 1;
        }
        else {
          cprintf("\rWrong parameter: %s\r\n", swch0);
          exit(1);
        }
      }

      else {
        cprintf("\rUnknown switch: %s\r\n", swch0);
        exit(1);
      }
    }

    else {
      strcpy(file_great, argv[k]);
    }
  }

  if (fl_e	== 1)
    fl_b = 1;

  if (fl_exa	== 1)
    fl_b = 1;

  if (fl_v	== 1) 
    fl_sam = 0;

  fl_sum = fl_e + fl_i + fl_ns + fl_yo + fl_npos;

  if (*file_great == '\0') {
    cprintf("\rЌҐ § ¤ ­ д ©«, ў Є®в®а®¬ ­ ¤® ­ ©вЁ ®Ўа §жл.\r\n");
    exit(1);
  }
  if (OUT2 == 0) {
    cprintf("\rЌҐ § ¤ ­ д ©« ¤«п ўлў®¤  аҐ§г«мв в  (/out:Ё¬п д ©« ).\r\n");
    exit(1);
  }

  if  (!fl_c) {
   if (!fl_g) {
     strcpy(file_litle, "fs.lst");
  }}

  n_str  = 0;
  f_ow   = 1;
  sw_tch = 0;

  if (*file_litle != '\0') {

    div_mask(file_litle, onlypath, onlymask);

    nf = scan_level(onlypath, onlymask, 0 + 100);

    strcpy(file_litle_rus, file_litle);
    conv_rus_OEMtoANSI(file_litle_rus);
    if (nf == 0) {
      if (!fl_si) cprintf("\r\n” ©«/л <%s> ­Ґ ­ ©¤Ґ­/л.\r\n", file_litle_rus);
    }
    else {
      if (!fl_si) cprintf("\r\nЌ ©¤Ґ­/® %ld д ©«/®ў <%s>\r\n", nf, file_litle_rus);
    }
  }

  if (*str_litle != '\0') {
    strcpy(string, str_litle);
    save_in_mem(string);
  }

  min_dl1 = min_dl - 1;

  if (n_rec == 0) {
    cprintf("\rЉ®«-ў® ®Ўа §ж®ў ¤«п Ї®ЁбЄ  а ў­® ­г«о.\r\n");
    exit(1);
  }

  if (!fl_si) cprintf("\r‘®авЁа®ўЄ .\r\n");
  sort_merge(p_beg, p_fin);
  if (!fl_si) cprintf("\r‘®авЁа®ўЄ  § ўҐаиҐ­ .\r\n");

  // Удаление похожих строк.
  // Cортировка такая, что при двух одинаково начинающихся строках
  // сначала идет более короткая строка.
  // Если есть две строки:
  //     ABCD
  //     ABCDEFG
  // И ключи /B и /E никак тут не влияют, все проверено,
  //  потому что если ключ /B - то это и так понятно.
  // А если ключ /E -  то строки уже перевернуьые, т.е.
  // DCBA
  // GFEDCBA

  // А если изначально были строки
  //        DEFG
  //     ABCDEFG

  // то при ключе /E в перевернутом виде это
  // GFED
  // GFEDCBA
  // strncmp даст совпадение и короткая строка поглотит длиную

  if (!fl_si) cprintf("\r“¤ «Ґ­ЁҐ ¤гЎ«ЁЄ в®ў ®Ўа §ж®ў.\r\n");
  del_dubl_1();
  if (!fl_si) cprintf("\rЋбв «®бм ®Ўа §ж®ў: %10ld\r\n", n_rec);

  // А вот как быть со строками
  //     CD
  //     ABCDEFG
  // По идее первая строка "поглощает" вторую.
  // А это просто - сделать fs "сам в себя". т.е. find_0
  // Но это нельзя делать, если есть ключ /B или /E
  // Вот реализация:

  // Удаление при вхождении одного образца в другой
/*
  if (!fl_si) cprintf("\rУдаление дубликатов образцов. Этап 2.\r\n");
  del_dubl_2();
  if (!fl_si) cprintf("\rОсталось образцов: %10ld\r\n", n_rec);
*/

  p_mid = bin_razbiv2(p_beg, p_fin);

  f_ow   = 2;
  sw_tch = 1;

  n_read     = 0;
  n_find     = 0;
  n_read_all = 0;
  n_find_all = 0;

  if (!fl_v) {
    return_when_found     = 1;
    return_when_not_found = 0;
  }
  else {
    return_when_found     = 0;
    return_when_not_found = 1;
  }

  div_mask(file_great, onlypath, onlymask);

  print_to_buff_ini();

//  nf = scan_level(onlypath, onlymask, fl_s);          // +100 говорит о том, что чтение файла не построчно, а поблочно
  nf = scan_level(onlypath, onlymask, fl_s + 100);      // +100 говорит о том, что чтение файла не построчно, а поблочно

  print_to_buff_the_end();

  strcpy(file_great_rus, file_great);
  conv_rus_OEMtoANSI(file_great_rus);

  if (nf == 0) {
    if (!fl_si) cprintf("\r\n” ©«/л <%s> ­Ґ ­ ©¤Ґ­/л.\r\n", file_great_rus);
  }
  else {
    if (!fl_si) cprintf("\r\nЌ ©¤Ґ­/® %ld д ©«/®ў <%s>.\r\n", nf, file_great_rus);
  }
  if (fl_cou != 0) {
    if (n_find_all2 == 0) {
      sprintf(Sn_find_all, "%lu", n_find_all);
    }
    else {
      sprintf(Sn_read_all, "%lu%lu", n_find_all2, n_find_all);
    }
    printf("Ќ ©¤Ґ­® бва®Є: %15s. ‚‘…ѓЋ.\n", Sn_find_all);
  }

  if (OUT2 > 0) {
    close(OUT2);
  }
  return 0;
}
