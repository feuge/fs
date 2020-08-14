#include <time.h>

int      IN2;
int      OUT2;
int      n_file;
int      n_dir;
char     masks[MXLEN];
char     mask [MXLEN];
char     prog [MXLEN];

/*
Это для того чтобы можно было подсунуть программе не одну маску файлов в командной строке,
а несколько масок в файле.
*/

struct mask_list {
       char          *mask;
       mask_list     *p_ml;
};
mask_list *p_ml_beg = NULL;
mask_list *p_ml_fin = NULL;
mask_list *p_ml     = NULL;


long int n_str       = 0;
long int n_str1      = 0;
long int n_str2      = 0;
long int n_str_all   = 0;
long int n_str_saved = 0;

unsigned char     fullfilename      [1300];
unsigned char     fullfilename_rus  [1300];
unsigned char     shortfilename     [1300];
unsigned char     shortfilename_rus [1300];
unsigned char     fullpathname      [1300];
unsigned char     fullpathname_rus  [1300];
unsigned char     extfilename       [1300];
unsigned char     extfilename_rus   [1300];
//unsigned char     onlypath         [1300];
//unsigned char     onlymask         [1300];

const unsigned char *eng_buk      = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";

// rus_ans  - это ANSI 866
const unsigned char *rus_ans      = " ЎўЈ¤Ґс¦§Ё©Є«¬­®ЇабвгдежзийклмнопЂЃ‚ѓ„…р†‡€‰Љ‹ЊЌЋЏђ‘’“”•–—™љ›њќћџь";
const unsigned char *rus_ans_sort = "Ґ ®­аЁб«ўЄв¬Јг¤Їплм©зЎе§¦жиондйск…ЂЋЌђ€‘‹‚Љ’Њѓ“„Џџ›њ‰—Ѓ•‡†–ћќ”™рљ№";

// rus_ans2 - это OEM 1251
const unsigned char *rus_oem      = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯь";
const unsigned char *rus_oem_sort = "еаонрислвктмгудпяыьйчбхзжцшюэфщёъЕАОНРИСЛВКТМГУДПЯЫЬЙЧБХЗЖЦШЮЭФЩЁЪ№";

#define N_MAX_PARTS 1024
char  *parts[N_MAX_PARTS];
/*------------------------------------------------------*/
int  f_ow;
void original_works            (struct ffblk *ffblk_current);
void original_works_binary_file(struct ffblk *ffblk_current); 
/*------------------------------------------------------*/

/*------------------------------------------------------*/
void to_upper(char *stri_signed) {

int j;
unsigned char *stri;

  stri = stri_signed;

  for (j = strlen(stri) - 1; j >= 0; j--) {
    if ((stri[j] >= 0xE0) /* && (stri[j] <= 0xFF) */ ) { /* русский диапазон */
      stri[j] = stri[j] - 0x20;
    }
    else
    if (stri[j] == 0xB8) {                               /* буква ё */
      stri[j] = 0xA8;
    }
    else
    if ((stri[j] >= 0x61) && (stri[j] <= 0x7A)) {        /* латинский диапазон */
      stri[j] = stri[j] - 0x20;
    }
  }
}

/*------------------------------------------------------*/
void to_upper_rus(char *stri_signed) {

int j;
unsigned char *stri;

  stri = stri_signed;

  for (j = strlen(stri) - 1; j >= 0; j--) {
    if ((stri[j] >= 0xE0) /* && (stri[j] <= 0xFF) */ ) { /* русский диапазон */
      stri[j] = stri[j] - 0x20;
    }
    else
    if (stri[j] == 0xB8) {                               /* буква ё */
      stri[j] = 0xA8;
    }
  }
}

/*------------------------------------------------------*/
void to_lower(char *stri_signed) {

int j;
unsigned char *stri;

  stri = stri_signed;

  for (j = strlen(stri) - 1; j >= 0; j--) {
    if ((stri[j] >= 0xC0) && (stri[j] <= 0xDF) ) { /* русский диапазон */
      stri[j] = stri[j] + 0x20;
    }
    else
    if (stri[j] == 0xA8) {                               /* буква ё */
      stri[j] = 0xB8;
    }
    else
    if ((stri[j] >= 0x41) && (stri[j] <= 0x5A)) {        /* латинский диапазон */
      stri[j] = stri[j] + 0x20;
    }
  }
}

/*------------------------------------------------------*/
void ANSI_or_OEM(unsigned char *stri, unsigned char *rus_asis, unsigned char *rus_tobe) {
int i, j, k;
  for (j = strlen(stri) - 1; j >= 0; j--) {
    for (k = 0; k < 66; k++) {
      if (unsigned (stri[j]) == unsigned (rus_asis[k])) {
        stri[j] = rus_tobe[k];
        break;
      }
    }
  }
}
void conv_rus_ANSItoOEM(unsigned char *stri) {
  ANSI_or_OEM(stri, (unsigned char *) rus_ans, (unsigned char *) rus_oem);
}
void conv_rus_OEMtoANSI(unsigned char *stri) {
  ANSI_or_OEM(stri, (unsigned char *) rus_oem, (unsigned char *) rus_ans);
}
void conv_rus_ANSItoOEM_sort(unsigned char *stri) {
  ANSI_or_OEM(stri, (unsigned char *) rus_ans_sort, (unsigned char *) rus_oem_sort);
}
void conv_rus_OEMtoANSI_sort(unsigned char *stri) {
  ANSI_or_OEM(stri, (unsigned char *) rus_oem_sort, (unsigned char *) rus_ans_sort);
}
/*------------------------------------------------------*/
void trim(char *str) {

int i2, l_s;

  for (l_s = strlen(str); l_s > 0; l_s--) {
    if (unsigned(str[l_s-1]) > unsigned(' ')) {
      break;
    }
    str[l_s-1] = '\0';
  }

  l_s = strlen(str);
  for (i2 = 0; i2 < l_s; i2++) {
    if (unsigned(str[i2]) > unsigned(' ')) {
      if (i2 > 0) {
        strcpy(str, str+i2);
      }
      break;
    }
  }
}

/*------------------------------------------------------*/
void rtrim(char *str) {

unsigned int i2, l_s;

  l_s = strlen(str);
  for (i2 = l_s; i2 > 0; i2--) {
    if (unsigned(str[i2-1]) <= unsigned(' '))
      str[i2-1] = '\0';
    else
      break;
  }
}

/*-----------------------------------------------------------*/
void ltrim(char *str) {

unsigned int i2, l_s;

  l_s = strlen(str);
  for (i2 = 0; i2 < l_s; i2++) {
    if (unsigned(str[i2]) > unsigned(' ')) {
      break;
    }
  }
  if (i2 > 0) {
    strcpy(str, str+i2);
  }
}
/*-----------------------------------------------------------*/
char onlypath[1300];
char onlymask[1300];

void div_mask(char *mask, char *onlypath_local, char *onlymask_local) {
  int i;

  *onlypath_local = '\0';
  strcpy(onlymask_local, mask);

  for (i = strlen(mask) - 1; i >= 0; i--) {
    if (mask[i] == '\\') {
      strcpy(onlymask_local, mask+i+1);
      strcpy(onlypath_local, mask);



//    03.06.2019:  Ѓл«® в Є: onlypath_local[i+1] = '\0';
      onlypath_local[i] = '\0';

      break;
    }
  }
}
/*********************************************************************/
int        read_first;			// Флаг первого чтения
void init_bin_file() {
  read_first = 1;
}
/*-----------------------------------------------------------*/
//unsigned long scan_level_recurcive(char *, char *, int);
unsigned long scan_level_file_counter; // Это глобальная переменная !!!
unsigned int  level_dir;               // Это глобальная переменная !!! 
/*-----------------------------------------------------------*/
// Открытие файлов в режиме TXT, вызов original_works на файл
// var ==  0 - сканирование файлов и папок без прохода  по вложенным папкам
// var ==  1 - сканирование файлов и папок c   проходом по вложенным папкам

// Без открытия файлов, без вызова original_works на файл
// var == 10 - сканирование файлов и папок без прохода  по вложенным папкам
// var == 11 - сканирование файлов и папок c   проходом по вложенным папкам

// Без открытия файлов, вызов original_works
// var == 20 - сканирование файлов и папок без прохода  по вложенным папкам. Но вообще то это нонсенс.
// var == 21 - сканирование файлов и папок c   проходом по вложенным папкам

// Или по другому:
// 0, 10, 20 - без прохода  по вложенным папкам
// 1, 11, 21 - с   проходом по вложенным папкам



// Если все тоже самое , но плюс 100 (100, 101, 110 и т.д.) - то все то же самое только файлы открываются не TXT, а BIN.



scan_level_recurcive(char *onlypath_local, char *onlymask_local, int var) {
  struct            ffblk ffblk;
  unsigned char     fullmask     [1300];
  unsigned char     new_path     [1300];
  unsigned char     new_path_rus [1300];
  unsigned char     fn_ful       [1300];
  unsigned char     fn_sho       [1300];
  int               done;
  unsigned int      i, j, k;

  level_dir++;  // Тут  в имени директории всегда есть концевой слэш.
  strcpy(fullmask, onlypath_local); strcat(fullmask, onlymask_local);

  /* Сканируем в текущей папке именно файлы */

  done = findfirst((char *) fullmask, &ffblk, 0);
  while (!done) {
    if ((ffblk.ff_attrib & FA_DIREC) == 0) {               /* Ґб«Ё ­ и«Ё д ©« */
  
      strcpy(fn_sho, ffblk.ff_name);
      strcpy(fn_ful, onlypath_local);
      strcat(fn_ful, fn_sho);
  
      strcpy(shortfilename,     fn_sho);
      strcpy(shortfilename_rus, fn_sho);
      conv_rus_OEMtoANSI(shortfilename_rus);

      *extfilename     = '\0';
      *extfilename_rus = '\0';

      for (i = strlen(shortfilename); i != 0; i--) {
        if (shortfilename[i-1] == '.') {
          strcpy(extfilename,     shortfilename     + i);
          strcpy(extfilename_rus, shortfilename_rus + i);
          break;
        }
      }
  
      strcpy(fullfilename,      fn_ful);
      if (strncmp(fullfilename, ".\\", 2) == 0) {
        strcpy(fullfilename, fullfilename+2);
      }
      strcpy(fullfilename_rus,  fullfilename);

      conv_rus_OEMtoANSI(fullfilename_rus);

      if ((var == 0) || (var == 1)){
        /* Нашли искомый файл, введем его в стандартный ввод */
        if ((in = fopen(fn_ful, "rt")) == NULL) {
          printf("ЌҐ ¬®Јг ®вЄалвм д ©« <%s>.\n",fullfilename_rus);
          exit(1);
        }
        original_works(&ffblk); /* оригинальная обработка открытого файла */
        fclose(in);
      }

      if ((var == 20) || (var == 21)){
        original_works(&ffblk); /* оригинальная обработка открытого файла */
      }

      if ((var == 100) || (var == 101)) {
        /* Нашли искомый файл, откроем как бинарный */

        IN2  = open(fn_ful , O_RDONLY|O_BINARY);

        if (IN2 == 0) {
          printf("ЌҐ ¬®Јг ®вЄалвм д ©« <%s>.\n",fullfilename_rus);
          exit(1);
        }
        init_bin_file();
        original_works_binary_file(&ffblk); /* оригинальная обработка открытого файла */
        close(IN2);
      }

      scan_level_file_counter++;
    }
    done = findnext(&ffblk);
  }

  if ((var == 1 ) || (var == 11) || (var == 21) || (var == 101)) {    // Будет погружение.

    /* Сканирование папок с погружением */
    
    strcpy(fullmask, onlypath_local);
    strcat(fullmask, "*.*");
    
    /* ‘Є ­ЁагҐ¬ ў вҐЄгйҐ© Ї ЇЄҐ Ё¬Ґ­­® Ї ЇЄЁ */
    
    done = findfirst((char *) fullmask, &ffblk, FA_DIREC);
    while (!done) {
    
      if (ffblk.ff_attrib & FA_DIREC) {
        if (strcmp(ffblk.ff_name, "." ) != 0) {
        if (strcmp(ffblk.ff_name, "..") != 0) {

          n_dir++;
          strcpy(new_path, onlypath_local);
          strcat(new_path, ffblk.ff_name);
    
          strcpy(fullpathname,      new_path);
    
          if (strncmp(fullpathname, ".\\", 2) == 0) {
            strcpy(fullpathname, fullpathname+2);
          }
          strcpy(fullpathname_rus,  fullpathname);
          conv_rus_OEMtoANSI(fullpathname_rus);
    
          strcat(new_path, "\\");
    
          if (var == 21) {
            original_works(&ffblk); /* ®аЁЈЁ­ «м­ п ®Ўа Ў®вЄ  ®вЄалв®© Ї ЇЄЁ */
          }
          scan_level_recurcive(new_path, onlymask_local, var);
        }
        }
      }
      done = findnext(&ffblk);
    }
  }

  level_dir--;
  return 0;
}
/*-----------------------------------------------------------*/
/* Џ®ЁбЄ д ©«®ў б Ї®Јаг¦Ґ­ЁҐ¬ ў Ї ЇЄЁ Ё Ї®¤Ї ЇЄЁ */
unsigned long scan_level(char *onlypath_local, char *onlymask_local, int var) {

unsigned char     onlypath2       [1300];
int               i;
// Это процедура только верхнего уровня.
// Рекурсивно файлы ищутся процедурой scan_level_recurcive.

  // Тут если в имени директории нет концевого слэша - добиваем его.

  if (    (var != 0) 
       && (var != 1) 
       && (var != 10) 
       && (var != 11) 
       && (var != 20) 
       && (var != 21) 
       && (var != 100) 
       && (var != 101)
  ) {
    printf("Wrong variant of parameter SCAN_LEVEL: %d\n", var);
    exit(1);
  }

  strcpy(onlypath2, onlypath_local);

  i = strlen(onlypath2);

  if  (i == 0) {
    strcpy(onlypath2, ".\\");
  }
  else
  if ((i == 1) && (onlypath2[0] == '\\')) {
    strcpy(onlypath2, ".\\");
  }
  else
  if ((i >= 1) && (onlypath2[i-1] != '\\')) {
    strcat(onlypath2, "\\");
  }

  scan_level_file_counter = 0;
  level_dir               = 0;
  scan_level_recurcive(onlypath2, onlymask_local, var);
  return scan_level_file_counter;
}

/*-----------------------------------------------------------*/
int	parse_string_csv(char *string) {
char	*c;
char	*p_null;
int	i, j, k;
int	nnn;	/* Найденное в исходной строке кол-во частей МИНУС ОДИН */

  p_null   = string + strlen(string);
  nnn      = 0;
  parts[0] = string;

  while(1) {		// По длинной строке


    // /////////////////////////////////////////////////////////////////////////////////
    // Если оставшийся хвост строки начинается на кавычки
    //    Если в нем есть сегмент КАВЫЧКИ+ТОЧКА-С-ЗАПЯТОЙ
    //       Сдвигаем начало хвоста на один символ вперед, т.е. эти кавычки пропускаем
    // Иначе просто ищем ТОЧКУ-С-ЗАПЯТОЙ
    //
    // Что-то тут не то. Если первые кавычки типа удаляются, то вторые то - нет.
    // И в стандартном варианте кавычки не ищутся.
    // 
    // Надо убрать анализ кавычек

    /*
    if (*parts[nnn] == '\042') {
      c = strstr (parts[nnn] + 1, "\042;");           // \042 - это кавычки
      if (c)
        c++;
      else {                                          // Это последняя часть.
      }
    }
    else {
      c = strstr (parts[nnn], ";");
    }
    */
    // /////////////////////////////////////////////////////////////////////////////////

    c = strstr (parts[nnn], ";");

    if (c) {
      *c = '\0';
      nnn++;
      parts[nnn] = c + 1;
      if (nnn == (N_MAX_PARTS - 1)) {
        break;
      }
    }
    else {
      break;
    }
  }

  for (i = nnn + 1; i < N_MAX_PARTS; i++) {
    parts[i] = p_null;
  }
  return nnn;
}
/*-----------------------------------------------------------*/
int	parse_string(char *string, char *ccc) {
char	*p;
char	*p_null;
int	i, j, k;
int	sl_s;
int	sl_c;
int	nnn;	/* Найденное в исходной строке кол-во разделителей */

  sl_s   = strlen(string);
  sl_c   = strlen(ccc);
  p_null = string + sl_s;
  nnn    = 0;
  parts[0] = string;

  for (i = 0; i < sl_s; i++) {		// По длинной строке которую надо разделить
    for (j = 0; j < sl_c; j++) {	// По которой строке разделителей
      if (string[i] == ccc[j]) {
        nnn++;
        parts[nnn] = string + i + 1;
        string[i] = '\0';
        break;
      }
    }
    if (nnn == (N_MAX_PARTS - 1)) {
      break;
    }
  }

  for (i = nnn + 1; i < N_MAX_PARTS; i++) {
    parts[i] = p_null;
  }
  return nnn;
}
/*-----------------------------------------------------------*/
int	parse_string_trim(char *string, char *ccc) {
char	*p;
char	*p_null;
int	i, j, k;
int	sl_s;
int	sl_c;
int	nnn;	/* Найденное в исходной строке кол-во разделителей */

  sl_s   = strlen(string);
  sl_c   = strlen(ccc);
  p_null = string + sl_s;
  nnn    = 0;
  parts[0] = string;

  for (i = 0; i < sl_s; i++) {
    for (j = 0; j < sl_c; j++) {
      if (string[i] == ccc[j]) {
        nnn++;
        parts[nnn] = string + i + 1;
        trim(parts[nnn - 1]);
        string[i] = '\0';
        break;
      }
    }
    if (nnn == (N_MAX_PARTS - 1)) {
      break;
    }
  }

  for (i = nnn + 1; i < N_MAX_PARTS; i++) {
    parts[i] = p_null;
  }
  return nnn;
}
/*-----------------------------------------------------------*/
int	parse_string_long_sep(char *string, char *long_sep) {
char	*p;
char	*p_null;
int	i, j, k;
int	sl_s;
int	sl_p;
int	nnn;	/* Найденное в исходной строке кол-во разделителей */

  sl_s   = strlen(string);
  sl_p   = strlen(long_sep);
  p_null = string + sl_s;
  nnn    = 0;
  parts[0] = string;

  p = string;
  while(1) {
    p = strstr(p, long_sep);
    if (p == NULL) {
      break;
    }
    memset(p, '\0', sl_p);
    p += sl_p;
    nnn++;
    parts[nnn] = p;
    if (nnn == N_MAX_PARTS - 1) {
      break;
    }
  }

  for (i = nnn + 1; i < N_MAX_PARTS; i++) {
    parts[i] = p_null;
  }
  return nnn;
}

/*-----------------------------------------------------------*/
void print_time() {
time_t t_9, t_0;
int    t_h, t_m, t_s;

/*
  t_9 = time(NULL);
  t_h = (t_9)/3600;
  t_m = (t_9-t_h*3600)/60;
  t_s = (t_9-t_h*3600-t_m*60);
  printf("%10d:%02d:%02d. %lu\n", t_h, t_m, t_s, t_9);
*/
  time(&t_9);
  printf("%s\n", ctime(&t_9));
}
/*-----------------------------------------------------------*/
clock_t  TIK=0, TIK0=0, TIK1=0, TIK2=0, TIK3=0, TIK4=0, TIK5=0, TIK6=0, TIK7=0, TIK8=0, TIK9=0;
clock_t  TIKI = 0;
clock_t  TIKI_PRED = 0;
clock_t  TIKI_FIRST = 0;
clock_t  TIKI_LAST = 0;
clock_t  tik;
long int n_ttt;
clock_t  TTT[100000];
long int stepik = 0;


void print_microtime(int fl) {

  tik = clock();
  if (fl != 0) {
    printf("%16lu\n", tik);
  }
  TIK0 = tik;
}
/*-----------------------------------------------------------*/
/* Удаление всех пробелов */
void delete_space (char *s) {
int l;
char *p;

  while (1) {
    p = strstr(s, " ");
    if (p == NULL)
      break;
    strcpy(p, p+1);
  }
}
/*------------------------------------------------------*/
int repl(char *str, char *s_old, char *s_new) {
int cha;
int l_old;
int l_new;
char *ccc;
char tmp[MXLEN];

  cha = 0;
  l_old = strlen(s_old);
  l_new = strlen(s_new);

  ccc = strstr(str, s_old);
  while (ccc) {
    if (l_old < l_new) {			// Строка расширяется:
      strcpy  (tmp, ccc + l_old);		//   Сохраняем хвост в отдельном месте
      strcpy  (ccc + l_new, tmp);		//   И уже оттуда пристраиваем хвост чуть-чуть правее
    }
    else
    if (l_old > l_new) {			// Строка сужается:
      strcpy  (ccc + l_new, ccc + l_old);	//   Смещаем хвост влево
    }
    strncpy (ccc, s_new, l_new);                //   Вписываем строку замены
    ccc = strstr(ccc + l_new, s_old);	        //   Новый поиск - с текущего места плюс длина подстроки подстановки
    cha++;
  }
  return cha;
}
/*-----------------------------------------------------------*/
/* Замена символов только в начале строки */
int repl_1(char *str, char *s_old, char *s_new) {
int i;
int l_old;
int l_new;
int l_str;
int n;

  l_old = strlen(s_old);

  if (strncmp(str, s_old, l_old) != 0)
    return 0;

  l_new = strlen(s_new);
  l_str = strlen(str);
  n     = l_str - l_old;                        // Кол-во перемещаемых символов

  if (l_new - l_old > 0) {			// Строка расширяется:
    for (i = n; i >= 0    ; i--) {              
      str[i + l_new] = str[i + l_old];          // Копируем посимвольно с конца
    }
  }
  else
  if (l_new - l_old < 0) {			// Строка сужается:
    for (i = 0; i <= n; i++) {              
      str[i + l_new] = str[i + l_old];          // Копируем посимвольно с начала
    }
  }

  strncpy (str, s_new, l_new);                  // Вписываем строку замены
  
  return 1;
}
/*-----------------------------------------------------------*/
/*
Этот вариант медленнее чем repl_yo.

int  conv_yo(char *str) {
char *cc;
int cha = 0;

  for (cc = str; (cc = strstr(cc,"Ё")) != 0; )
    *cc = 'Е';
  for (cc = str; (cc = strstr(cc,"ё")) != 0; )
    *cc = 'е';
  return cha;
}
*/
/*-----------------------------------------------------------*/
int repl_yo(char *str) {
int rx  = 0;
int cha = 0;

  while (str[rx] != '\0') {
    if (str[rx] == 'Ё') str[rx] = 'Е', cha++;
    if (str[rx] == 'ё') str[rx] = 'е', cha++;
    rx++;
  }
  return cha;
}
/**************************************************************************/
/* Сравнение двух неравных по длине строк; строки сравниваются по длине более короткой строки */
int str_like(char *str1, char *str2) {
int l, l1, l2;

  l1 = strlen(str1);
  l2 = strlen(str2);
  if (l1 < l2)
    l = l1;
  else
    l = l2;
  if (l == 0)
    return 0;

//printf("COMPARE 1.<%s> и 2.<%s>. Длина = %3d. Рез-т: %3d\n", str1, str2, l,  strncmp(str1, str2, l));

  if (strncmp(str1, str2, l) == 0)
    return 1;
  else
    return 0;

}
/**************************************************************************/
unsigned long int temeix(int dd, int mm, int yy) {
unsigned long dt;
time_t   sec;
struct   tm tmx;

  tmx.tm_mday  = dd;
  tmx.tm_mon   = mm - 1;
  tmx.tm_year  = yy - 1900;
  tmx.tm_hour  = 0;
  tmx.tm_min   = 0;
  tmx.tm_sec   = 0;
  tmx.tm_isdst = -1;

  sec = mktime(&tmx);
  dt  = sec;
  return dt;

}
/********************************************************************/
/* Является ли символ цифрой ? */

int is_cifra(char x) {

  if (x >= '0')
    if (x <= '9')
      return 1;
  return 0;
}

/********************************************************************/

void sec_to_dat(time_t sec, char *dat) {
struct tm *tim;

  tim = localtime(&sec);
  strftime(dat, 80, "%d.%m.20%y", tim);
}
/*-----------------------------------------------------------*/
unsigned long dat_to_sec(char *dat, char *tim) {

char     ddd[10];
time_t   sec;
struct   tm tmx;
unsigned int dt;
unsigned int l;


  l = strlen(dat);
  if ((l != 10) && (l != 8)) {
    return 0;
  }

  strncpy(ddd, dat+0, 2);
  ddd[2] = '\0';
  tmx.tm_mday  = atoi(ddd);

  strncpy(ddd, dat+3, 2);
  ddd[2] = '\0';
  tmx.tm_mon  = atoi(ddd) - 1;

  if (l == 8) {
    strncpy(ddd, dat+6, 2);                     /* только две цифры года */
    ddd[2] = '\0';
    tmx.tm_year = atoi(ddd) + 2000 - 1900;
  }
  else
  if (l == 10) {
    strncpy(ddd, dat+6, 4);                     /* только две цифры года */
    ddd[4] = '\0';
    tmx.tm_year = atoi(ddd) - 1900;
  }

  strncpy(ddd, tim+0, 2);
  ddd[2] = '\0';
  tmx.tm_hour  = atoi(ddd);

  strncpy(ddd, tim+3, 2);
  ddd[2] = '\0';
  tmx.tm_min   = atoi(ddd);

  strncpy(ddd, tim+6, 2);
  ddd[2] = '\0';
  tmx.tm_sec   = atoi(ddd);

  tmx.tm_isdst = -1;

  sec = mktime(&tmx);
  dt  = sec;
  return dt;

}

/*-----------------------------------------------------------*/
int may_be_acc_inn(char *mba) {

int i,j,k;
int i2;
char *mbc;

  i2 = strlen(mba);

  if ((i2 != 10) && (i2 != 12) && (i2 < 15)) {
    return 0;
  }

  for (mbc = mba; *mbc != '\0'; mbc++) {
    if (*mbc < '0') 
      return 0;             /*   - возвращаем ноль */
    if (*mbc > '9') 
      return 0;             /*   - возвращаем ноль */
  }
  return 1;
}
/*-----------------------------------------------------------*/
int may_be_int(char *mbn_in, long &num) {

int  i,j,k;
int  i1, i2;
char mbn[MXLEN];
char *mbc;

  num = 0;
  strcpy(mbn, mbn_in);
  trim(mbn);
//  if (*mbn == '0') {      // Если есть лидирующий ноль, то это не число
//    return 0;
//  }

  i1 = 0;
  i2 = strlen(mbn);

  if (i2 == 0) {
    return 0;
  }
  if (i2 > 100) {
    return 0;
  }

  if (*mbn == '+')
    i1++;
  else
  if (*mbn == '-')
    i1++;

  if (mbn[i1] == '\0') {   // Если в строке кроме минуса или плюса ничего нет - это не число */
    return 0;
  }

  if (i2 - i1 >= 10) { // При кол-ве цифр >= 10 теряется точность.
    return 0;
  }

  for (mbc = mbn; *mbc != '\0'; mbc++) {
    if (*mbc < '0')
      return 0;             /*   - возвращаем ноль */
    if (*mbc > '9')
      return 0;             /*   - возвращаем ноль */
  }
  num = atol(mbn);          /* Только цифры -возвращаем 1 */
  return 1;
}
/*-----------------------------------------------------------*/
int may_be_float(char *mbf_in, double &flo) {

int  i,j,k;
int  p_c;
int  i1, i2, i3;
char *point;
char mbf[MXLEN];
char *mbf_i;

  flo = 0;
  strcpy(mbf, mbf_in);
  trim(mbf);
//  if (*mbf == '0') {      // Если есть лидирующий ноль, то это не число
//    return 0;
//  }

  i2  = strlen(mbf);

  if (i2 == 0) {
    return 0;
  }
  if (i2 > 100) {
    return 0;
  }

  i1  = 0;
  if (*mbf == '+')
    i1 = 1;
  else
  if (*mbf == '-')
    i1 = 1;

  if (mbf[i1] == '\0') {       // Если в строке кроме минуса или плюса ничего нет - это не число */
    return 0;
  }

  if (mbf[i1+1] == '\0') {   
    if (mbf[i1] == '.') {         // Если в строке только точка - это не число */
      return 0;
    }
    if (mbf[i1] == ',') {         // Если в строке только точка - это не число */
      return 0;
    }
  }

  point = strstr(mbf, ".");
//  if (point == NULL)
//    point = strstr(mbf, ",");
  if (point == NULL)
    point = mbf + i2;

  i3 = (point - mbf) - i1;

  // i2 - i1         - кол-во цифр, без знака плюс или минус
  // i2 - point      - кол-во цифр перед точкой
  // i2 - i1 - point - кол-во цифр перед точкой, без знака плюс или минус

  if (i3 >= 16) { // При кол-ве цифр >= 16 теряется точность.
    return 0;
  }

  p_c = 0;

  // i1 - сдвиг на знак '+' или '-'

  for (mbf_i = mbf + i1; *mbf_i != '\0'; mbf_i++) {
//    if ((*mbf_i == '.') || (*mbf_i == ',')) {
    if ((*mbf_i == '.') || (*mbf_i == '.')) {
      p_c++;
      if (p_c > 1) {          /* В строке нашлось две точки/запятые */
        return 0;
      }
    }
    else {
      if (*mbf_i < '0') {
        return 0;
      }

      if (*mbf_i > '9') {
        return 0;
      }
    }
  }

  // atof понимает только символ "точка"
  // А мы хотим, чтобы и запятая тоже понималась как разделитель целой и дробной частей

  if (*point != '\0') 
    *point = '.';
  flo = atof(mbf);

  return 1;
}

/*-----------------------------------------------------------*/
// Процедура пересчитывает дату в количество дней от Рождества Христова
int may_be_date(char *mbd_in, unsigned long int &dat_ld) {

char              gg[20], mm[20], dd[20];
char              mbd[200];
int               s_l;
int               i, j, k, lg;
int               m;
int               visokos;
long              num;
struct tm tmx;
//   Месяцы:        я   ф   м   а   м   и   и   а   с   о   н   д
//   Кол-во дней:
long int dm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
long int ggx, mmx, ddx;

  dat_ld = 0;
  s_l = strlen(mbd_in);

  if ((s_l != 8) && (s_l != 10)) {
    return 0;
  }

  strcpy(mbd, mbd_in);
  repl(mbd, "-", ".");
//  repl(mbd, "/", ".");

  if (dat_ld != 0) {
  }  // Дальше - ELSE

  /* Проверяем на формат DD.MM.YY  , DD/MM/YY  , DD-MM-YY   */
  else if ((s_l ==  8) && (mbd[2] == '.') && (mbd[5] == '.')) i = 0, j = 3, k = 6, lg = 2;

  /* Проверяем на формат DD.MM.YYYY, DD/MM/YYYY, DD-MM-YYYY */
  else if ((s_l == 10) && (mbd[2] == '.') && (mbd[5] == '.')) i = 0, j = 3, k = 6, lg = 4;

  /* Проверяем на формат YYYY.MM.DD, YYYY/MM/DD, YYYY-MM-DD */
  else if ((s_l == 10) && (mbd[4] == '.') && (mbd[7] == '.')) i = 8, j = 5, k = 0, lg = 4;
  else {
    return 0;
  }

  strncpy(dd, mbd+i, 2); dd[2] = '\0';
  strncpy(mm, mbd+j, 2); mm[2] = '\0';

  /*
     Если дата в формате дд.мм.гг - какой век прибавить?
     Посмотрим.
     01.01.00     - это скорее всего 01.01.2000
     01.01.10     - это скорее всего 01.01.2010
     01.01.20     - это уже будущее.
			Если это - д-р, то тогда                  01.01.1920
			А если дата окончания договора - то тогда 01.01.2020
     01.01.30     - это скорее всего 01.01.1930
     01.01.40     - это скорее всего 01.01.1940
     01.01.50     - это скорее всего 01.01.1950
     01.01.60     - это скорее всего 01.01.1960
     01.01.70     - это скорее всего 01.01.1970
     01.01.80     - это скорее всего 01.01.1980
     01.01.90     - это скорее всего 01.01.1990

     В общем из ЕКС (и других источников) даты надо выгружать в формате 'DD.MM.YYYY'
  */

  strncpy(gg, mbd+k, lg);
  gg[lg] = '\0';

  // надо еще проверить на ЦИФИРЬ, потому что вот это -  "+1.06.+301" - не дата.
  if (!is_cifra(dd[0]))         return 0;
  if (!is_cifra(mm[0]))         return 0;
  if (!is_cifra(gg[0]))         return 0;

  if (may_be_int(dd, ddx) != 1) return 0;
  if (may_be_int(mm, mmx) != 1) return 0;
  if (may_be_int(gg, ggx) != 1) return 0;

  visokos = (ggx % 4 == 0) && (ggx % 100 != 0) || (ggx % 400 == 0);

  // Еще естественные ограничения.
  if (ggx == 0)                 return 0;
//  if (ggx < 1800)               return 0;
//  if (ggx > 2200)               return 0;

  // Месяц - от 1 до 12.
  if (mmx == 0)                 return 0;
  if (mmx > 12)                 return 0;

  // Кол-во дней - в каждом месяце разное.
  if (ddx == 0)                 return 0;

  if ((visokos) && (mmx == 2)) {
    // Год - високосный, месяц - февраль.
    //   Если дней больше 29 - return 0; 
    if (ddx > 29)               return 0; 
  }


  else {
    //Либо год не високосный, либо месяц - не февраль.
    if (ddx > dm[mmx-1])        return 0; //
  }

  // Вычислим кол-во дней в полностью завершенных годах.
  // Например, если указана дата 06.04.2011 - значит 2010 год полностью закончен
  // Из 2011 отнимаем единицу и рассчитываем.
  // Прибавляем еще високосные дни в каждом четвертом году.
  dat_ld = (ggx-1) * 365 + (ggx-1)/4 - (ggx-1)/100 + (ggx-1)/400;

  // mmx - текущий месяц. Который еще не закончился. Месяц заканчивается, конда наступает следующий месяц.
  // Надо просуммировать кол-во дней в предудущих месяцах.
  // Если сейчас апрель, суммируем январь-февраль-март.
  // Если сейчас январь - ничего не считаем.

  for (m = 1; m < mmx ; m++) {
    dat_ld += dm[m-1];
  }

  // Если текущий месяц - високосный, и указан месяц март или больше - добавим еще один день - 29.02
  if ((visokos) && (mmx > 2)) {
    dat_ld++;
  }

  dat_ld += ddx;

  return 1;
}

/********************************************************************/
int may_be_time(char *mbt_in, unsigned long int &tim_ld) {

char              mbt[20];
int               s_l;
int               i, j, k;
int               h, m, s;
long              num;

  tim_ld = 0;
  s_l = strlen(mbt_in);
  *mbt = '\0';

  if (s_l == 0) {
    return 0;
  }
  else if (s_l == 4) {
    strcpy(mbt, "0");    strcat(mbt, mbt_in);    strcat(mbt, ":00");
  }
  else if (s_l == 5) {
                         strcat(mbt, mbt_in);    strcat(mbt, ":00");
  }
  else if (s_l == 7) {
    strcpy(mbt, "0");    strcat(mbt, mbt_in);
  }
  else if (s_l == 8) {
                         strcpy(mbt, mbt_in);
  }
  else {
    return 0;
  }

  if ((1==1)
      && (is_cifra(mbt[0]))
      && (is_cifra(mbt[1]))
      && (mbt[2] == ':')
      && (is_cifra(mbt[3]))
      && (is_cifra(mbt[4]))
      && (mbt[5] == ':')
      && (is_cifra(mbt[6]))
      && (is_cifra(mbt[7]))
     ) {
     mbt[2] = '\0';
     mbt[5] = '\0';
     h = atoi(mbt+0); if (h >= 24) return 0;
     m = atoi(mbt+3); if (h >= 60) return 0;
     s = atoi(mbt+6); if (h >= 60) return 0;

     tim_ld =  h * 3600 + m * 60 + s;
  }
  else {
    return 0;
  }

  return 1;
}
/********************************************************************/
int may_be_datetime(char *mbdt_in, unsigned long int &dat, unsigned long int &tim) {

int     s_l;
int     dl_d = 0;
int     dl_t = 0;
char	dtdt[20];

  // Дата м.б. длиной 8 или 10, время - 7 или 8. Плюс 1 пробел
  // Т.о.  8+7+1 = 16
  // Т.о.  8+8+1 = 17
  // Т.о. 10+7+1 = 18
  // Т.о. 10+8+1 = 19
  s_l = strlen(mbdt_in);

  switch (s_l) {
  case 16:
           dl_d =  8; dl_t = 7;
           break;
  case 17:
           dl_d =  8; dl_t = 8;
           break;
  case 18:
           dl_d = 10; dl_t = 7;
           break;
  case 19:
           dl_d = 10; dl_t = 8;
           break;
  }


  if (mbdt_in[dl_d] != ' ') {
    return 0;
  }

  strncpy(dtdt, mbdt_in        + 0, dl_d); dtdt[dl_d] = '\0';
  if (may_be_date(dtdt, dat) == 0) {
    return 0;
  }

  strncpy(dtdt, mbdt_in + dl_d + 1, dl_t); dtdt[dl_t] = '\0';
  if (may_be_time(dtdt, tim) == 0) {
    return 0;
  }

  return 1;
}

/*********************************************************************/
void num_to_dat(unsigned long int num, char *dat, char *frmt) {
unsigned long int i;
unsigned long int tetra;
unsigned long int gg = 0;
unsigned long int mm = 0;
unsigned long int dd = 0;
unsigned long int x1, x2, x3;

unsigned long int dm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int               vv, dop;
unsigned long int d400;
unsigned long int d100;
char              r;

  // 01.01.0001 в переводе в NUM равно 1.

  // Тетрада начинается 01.01.0001 и заканчивается 31.12.0004
  // В тетраде високосный - последний (четвертый) год.

  // Если num = 1500, то это уже 0005 год.

  // Високосными годами являются года, делящиеся на 4,
  //   за исключением годов, делящихся на 100;
  //     но год, делящийся на 400 - тоже високосный.

  // Сколько дней в одном отрезке в 400 лет?
  // 365 * 400 + 97 

  if (num != 0) {
    if (num > 3652059) {   // это равно 31.12.9999
      num = 3652059;
    }
    
    dd = num;

    // Отсчитываем полные отрезки в 400 лет
    d400 = 365 * 400 + 97;
    while (dd > d400) {
      dd = dd - d400;
      gg = gg +  400;
    }
    
    // Сейчас в dd - хвост не более 400 лет.
    // в столетии [  0.. 99] 36500 + 1 день. Потому что год [0] без остатка делится на 400.
    // в столетии [100..199] 36500     день.
    // в столетии [200..299] 36500     день.
    // в столетии [300..399] 36500     день.
    // 
    
    // d100 - это кол-во дней в одном 100-летнем периоде.
    // Но в первом из четырех сто-летних периодов на один високосный год больше.
    
    if (dd > 36524) gg += 100, dd -= 36524;
    if (dd > 36524) gg += 100, dd -= 36524;
    if (dd > 36524) gg += 100, dd -= 36524;
    
    tetra = (dd - 1)/1461;  // 1461 - кол-во дней в одной тетраде
    
    gg = gg + tetra * 4 + 1;
    dd = dd - tetra * 1461;
    
    vv = 0;
    if (dd > 365) gg++, dd -= 365;
    if (dd > 365) gg++, dd -= 365;
    if (dd > 365) gg++, dd -= 365, vv++;
    
    // Если v == 4, значит мы дошли до возможно високосного года
    // Но не всякий год, делящийся на 4 - високосный.
    // Если год делится на 100 и не делится на 400 - это не високосный
    
    if (vv == 1) 
      if ((gg % 100) == 0) 
        if ((gg % 400) != 0) 
          vv = 0;
    
    dop = 0;
    for (mm = 1; dd > dm[mm - 1] + dop; mm++) {
      dd -= (dm[mm - 1] + dop);
    
      dop = 0;
      if (mm == 1)                // В високосном году один дополнительный день
        dop = vv;			
    }
    // Либо день впереди, год позади, либо наоборот. 
  }

  r = frmt[0];

  if (strcmp(frmt+1, "DMY") == 0)  { sprintf(dat, "%02d%c%02d%c%04d", dd, r, mm, r, gg); }
  else 
  if (strcmp(frmt+1, "YMD") == 0)  { sprintf(dat, "%04d%c%02d%c%02d", gg, r, mm, r, dd); }

}
/*********************************************************************/
void num_to_tim(unsigned long int num, char *tim) {
unsigned long int hh, mm, ss;
unsigned long int mm_ss;

//  tim_ld = hh*3600 + mm*60 + ss;
  hh  = num / 3600;                          // Часы
  num = num - 3600*hh;                       // Остались минуты с секундами

  mm  = num /   60;                          // Минуты
  num = num -   60*mm;                       // Остались секунды

  ss  = num;
  sprintf(tim, "%02d:%02d:%02d", hh, mm, ss);
}

/*********************************************************************/
void tetroid (double num, char *str) {
  char str0[1000];
  int  ls;
  int i, j, x;

  sprintf(str0, "%.0f", num);
  ls = strlen(str0);
  for (x = ls; x > 3; x = x - 3) {
  }

  for (i = 0, j = 0; i < ls; i++, j++) {
    str[j] = str0[i];
    x--;
    if (x == 0) {
      j++;
      str[j] = ',';
      x = 3;
    }
  }
  str[j-1] = '\0';
}


/*********************************************************************/
void include_in_kav(char *str) {
  int l;
  char tmp_str[200];

  l = strlen(str);
  if ((l >= 12) && (l <= 100)) {
    strcpy(tmp_str, "=\042");
    strcat(tmp_str, str);
    strcat(tmp_str, "\042");
    strcpy(str, tmp_str);
  }
}


/*********************************************************************/
int glob_17 = 0;
//const unsigned char *long_buk      = ";.№ь0123456789еаонрислвктмгудпяыьйчбхзжцшюэфщёъЕАОНРИСЛВКТМГУДПЯЫЬЙЧБХЗЖЦШЮЭФЩЁЪҐ ®­аЁб«ўЄв¬Јг¤Їплм©зЎе§¦жиондйск…ЂЋЌђ€‘‹‚Љ’Њѓ“„Џџ›њ‰—Ѓ•‡†–ћќ”™рљqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
const unsigned char *long_buk      = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzЂЃ‚ѓ„…†‡€‰Љ‹ЊЌЋЏђ‘’“”•–—™љ›њќћџ ЎўЈ¤Ґ¦§Ё©Є«¬­®Їё№АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя0123456789 !	\042#$%&'()*+,-.;/:<=>?@[\\]^_`{|}~";

int shifrobuk(char *str, int f_act) {

int ls;
int lb;
int i;
int j;
int k;
int fl_sh;

  ls = strlen(str);
  lb = strlen(long_buk);

  fl_sh = 0;
  for (i = 0; i < ls; i++) {

/*
    if ((unsigned char)str[i] < ...) {
      continue;
    }
    if ((unsigned char)str[i] > ...) {
      continue;
    }
*/
    for (j = 0; j < lb; j++) {
      if ( (unsigned char)str[i] == (unsigned char)long_buk[j] ) {
        fl_sh = 1;
        glob_17 = glob_17 + 17;
        if (glob_17 >= lb) {
          glob_17 = glob_17 - lb;
        }
        if (f_act == 0) {
          k = j + glob_17;
          if (k >= lb) {
            k = k - lb;
          }
        }
        else {
          k = j - glob_17;
          if (k < 0) {
            k = k + lb;
          }
        }

//        printf("%c;%c;%3d;%3d;%3d;%3d\n", str[i], long_buk[k], i, j, glob_17, k);
        (unsigned char)str[i] = (unsigned char)long_buk[k];
        break;
      }
    }
  }
  return fl_sh;
}

/*********************************************************************/
// Глобальные переменные:
const int DL_BUFF_IN = 4096000;
char      buff_in[DL_BUFF_IN];		// Буфер чтения
char      buff_in_theend[100];		// Это всего лишь прослойка
int       dl_buff_in;
char      *buff_in_str_beg;		// Начало текущей строки


//////////////////////////////////
struct_str_spec fgets_from_bin_file(int IN2) {

int              readeno;			// Счетчик считанных байт
int              dl_hvost;
char             *xx;
char             ps;
struct_str_spec  str_wl;

  // Самое первое чтение файла
  if (read_first == 1) {
    read_first = 0;
    readeno = read(IN2, buff_in, DL_BUFF_IN);
    if (readeno <= 0) { // "0" - прочитано 0 байтов, "-1" - какие-то проблемы с чтением.
      str_wl.str = NULL;
      str_wl.len = 0;
      return str_wl;
    }
    buff_in_str_beg = buff_in;	
    dl_buff_in      = readeno;
  }

  // Если на предыдущем шаге файл закончился 
  if (buff_in_str_beg == NULL) {
    str_wl.str = NULL;
    str_wl.len = 0;
    return str_wl;
  }

  // Попробуем "вытащить" из того, что уже прочитано, еще одну текстовую строку
  ps = '\n';
  for (xx = buff_in_str_beg; ; xx++) {

    if (xx < buff_in + dl_buff_in) {           // 
      if (*xx == ps) {                         // Нашли \n
        *xx = '\0';
        str_wl.str = buff_in_str_beg;
        str_wl.len = xx - buff_in_str_beg;
        buff_in_str_beg = xx + 1;
        xx--;
        while (*xx == '\r') {
          *xx = '\0';
          xx--;
          str_wl.len--;
        }  
        return str_wl;
      }
    }

    if (xx == buff_in + dl_buff_in) {        // Дошли до конца буфера, не найдя '\n'
      if (dl_buff_in < DL_BUFF_IN) {         //   а буфер укороченный, т.е. в последний раз был считан хвост файла. 

        *xx = '\0';
        str_wl.len = xx - buff_in_str_beg;
        
        if (str_wl.len == 0) {
          str_wl.str = NULL;
        }
        else {
          str_wl.str = buff_in_str_beg;
        }
        buff_in_str_beg = NULL;
        dl_buff_in      = 0;
        
        return str_wl;
      }

      if (buff_in_str_beg == buff_in) {      // Мы целиком прошли весь буфер и не нашли ни одного символа '\n'
        printf("Оченно длинная строка.\n");
        exit(1);
      }

      // Надо Перекинуть хвост в начало буфера и прочитать новый блок. Но чтобы буфер получился не более чем 4096000.
      // Но если длина пройденного больше или равна DL_buff_in
      // То это значит что у нас попалась Очень Длинная Строка
  
      //if (dl_hvost == 0) 

      dl_hvost = xx - buff_in_str_beg;					// Длина хвоста
      memcpy(buff_in, buff_in_str_beg, dl_hvost);			// Перемещаем хвост в начало

      readeno = read(IN2, buff_in + dl_hvost, DL_BUFF_IN - dl_hvost);	// Дочитываем еще блок
      buff_in_str_beg = buff_in;			        	// Начало просмотра
      xx              = buff_in;                              
      dl_buff_in      = dl_hvost + readeno;			        // Длина буфера с дочиткой
    }
  }
}

/*********************************************************************/
// Глобальные переменные:
int       feof_OUT2; 	 			// Флаг конца файла
const int DL_BUFF_OUT = 4096000;
char      buff_out[2*DL_BUFF_OUT];		// Буфер накопления
char      *buff_out_theend;			// Конец Буфер накопления (указывает на терминальный ноль)
int       buff_out_dl;
////////////
void print_to_buff_ini() {
  buff_out_theend = buff_out;
  buff_out_dl     = 0;
}
////////////
void print_to_buff_the_end() {

  if (OUT2 != 0) {
    write(OUT2, buff_out, buff_out_dl);				// В конкретный файл
    print_to_buff_ini();
  }
}
////////////
void print_to_buff(char *string_to_buff, char *slash_n) {
int dls;

  if (OUT2 == 0) {
    printf("%s%s", string_to_buff, slash_n);
    return;
  }

  dls = strlen(string_to_buff);
  strncpy(buff_out_theend, string_to_buff, dls);
  buff_out_dl     = buff_out_dl     + dls;		// Увеличиваем длину заполненного буфера
  buff_out_theend = buff_out_theend + dls;		// Смещаем конец буфера

  if (*slash_n == '\n') {
    *(buff_out_theend + 0) = '\r'; 
    *(buff_out_theend + 1) = '\n'; 
    buff_out_dl     += 2;		// Увеличиваем длину заполненного буфера
    buff_out_theend += 2;		// Смещаем конец буфера
  }

  *buff_out_theend = '\0'; 

  if (buff_out_dl >= DL_BUFF_OUT) {
    print_to_buff_the_end();
  }
}     
/*------------------------------------------------------*/
void press_space(char *s_in) {
char *s0;    // Указатель на первый непробел
char *s1;    // Скользной, источник
char *d;     // Скользной, приемник


  /* LTRIM. Поиск первого непробела */

  for (s0 = s_in; *s0 != '\0'; s0++) {
    if (*s0 != ' ')
      break;
  }

  /* Поиск двух и более пробелов */
  for (s1 = s0, d = s0; *s1 != '\0'; s1++) {
    if ( (*(s1) == ' ') &&  (*(s1+1) == ' ') ) {
      continue;
    }
    else {
      *d = *s1;
      d++;
    }
  }

  *d = '\0';

  /* LTRIM. Завершение */
  if (s0 != s_in)
    strcpy(s_in, s0);

  return;
}
