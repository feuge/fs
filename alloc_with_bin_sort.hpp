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
      insert_new_rec();		// Insert new record (p_new) ������������ P_BFL
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
    if (!fs) {                  // ������� - ��� !!!
      create_new_rec();		// Create new record (p_new)
      insert_new_rec();		// Insert new record (p_new) ������������ P_BFL
    }
  }
}
/*-----------------------------------------------------------*/
unsigned int bf_n_extreme     	= 0;		// ���-�� binary_finding � ������ extreme
unsigned int c2r_extreme_c  	= 0;		// ���-�� �������  cmp2rec � ������ extreme
unsigned int c2r_extreme_l	= 0;		// �������� �����  extreme
unsigned int c2r_extreme_r	= 0;		// �������� ������ extreme
unsigned int c2r_extreme_e	= 0;		// � ������ extreme ������ �� ���������

unsigned int bf_n_bintree     	= 0;		// ���-�� binary_finding � ������ bintree
unsigned int c2r_bintree_c  	= 0;		// ���-�� �������  cmp2rec � �������� ������
unsigned int c2r_bintree_l	= 0;		// ������ �����  ��� �� ��������� ������
unsigned int c2r_bintree_r	= 0;		// ������ ������ ��� �� ��������� ������
unsigned int c2r_bintree_e	= 0;		// � ����� ������ ������ �� ���������
/*-----------------------------------------------------------*/
void binary_finding(t_d_s *p_fnd) {

int   f_cyc;
long  fs_loc; 
t_d_s *p_tmp;

/*		��� �������� ������ ����������� ����.
  if (!n_rec) {
    return NULL;
  }
*/

// ���� ���������� ������� ��� � ����� �����  ��, � ������� ������� - ���� � ����� �����  ��, ��
// ���� ����� ��������� ����� ������ � �����  �� 

//  f_cyc = 0 - ��� ��������, ��� ������� ���-�� � �������� ��

//  ���� ������, ��� ������ ������������ �� �����������, � ����� ������ ���� �������� � �����. 
//  ������� ������ ����� ��������� ������ ���� ��������.

//  fl_extreme = 0;


  if (fl_extreme) {    
    // � ���������� ��� ����� ������ ������ � ����� ����.

    bf_n_extreme++;

    p_ii2  = p_x_hi2;
    p_ii1  = p_x_hi1;
    p_ii0  = p_x_hi0;

    c2r_extreme_c++; // ����������
    fs = cmp2rec(p_fnd, p_ii0);                // ��������� ������ �������� � ����������, ������� ��� ���������� � ����

    if (!fs) {                                 // ���� fs ����� ����, �� on_equal
      on_equal_when_binary_finding(p_ii0);     // �� ��������, ��� � on_equal fs ����� ���������� �� -1 ��� +1 
      c2r_extreme_e++;
    }
    else 
    if ((fs ^ fl_extreme) < 0) {               // ���� fs �������������� �� ����� fl_extreme
      fl_extreme = 0;                          //   �� ����� ������ � ��������
    }                                          
    else { // ����������
      if (fl_extreme > 0)
        c2r_extreme_r++;
      else
      if (fl_extreme < 0)
        c2r_extreme_l++;
    }
  }                                            

  if (!fl_extreme) {                           // �.�. if (fl_extreme == 0)
    bf_n_bintree++;
    p_ii0  = p_mid;
    p_ii1  = NULL;
    p_ii2  = NULL;
    fs     = 0;
    
    while (1) {

      n_c3++;  // �� ������������, ���� ������� ����� ��� ��������

      fs_1  = fs;                  // ��������� ������� ���������

      c2r_bintree_c++;             // ����������
      fs = cmp2rec(p_fnd, p_ii0);  // ����������.

// � binary_finding ���� ����: �����
// �������������� �� ������ ������ ���� ���� ��������� �� ��������� ������� ���� NULL

// ������� �������� ��� ������ ���������:
// 1. ����������
// 2. �� ���������, ��������������
// 3. �� ���������, ������ �� ������

// ����� �������� ��� � on_full_equal
// � ����������� ������� on_full_equal - ������.
// ������ ��� ��������� � �������� �1.
// ������, ����� ����� �� ������� �1, 
//   ���� ���� � on_full_equal ��������� fs = 1 
//   (���� ������� ������ �������� ������ �� �������������)
//   ��� fs = -1 
//   (���� ������� ������ �������� �����)

// ����� � ������ ���������� �� ����������, ������� ����� on_full_equal �� on_equal

// ��� �������� 1 ������ ������ BREAK, ���� ����� �� �������� ��������  !!!
// ��� �������� 2 ���� �������  BREAK, �� �������� �������� ���� �� ����!!!
// ��� �������� 3 ���� �������  BREAK, �� �������� �������� ���� �� ����!!!
      if (!fs) {
        on_equal_when_binary_finding(p_ii0);
// � on_equal() fs ������ ����������:
// ��� ������� ������ - fs = 1, ����� - -1.
// ��� ������������ ��� ������ - fs �� �������� (�������� ������ ����).
      }

      fs_loc = fs;       // fs_loc �������� �� ��������!!!
      if (fs_loc == 0) {
        c2r_bintree_e++; // ����������
        break;           // ����� �� ����� ��� ������ ��������� � ������ ������������ ��� ������
      }
      else 
      if (fs_loc < 0) {
        c2r_bintree_l++; // ����������
        p_tmp = p_ii0->p_poi[2];
        if (!p_tmp) {    // ����� �� ����� ����� ����� �� �������� ��������                  
          v_v = 2;
          break;
        }
      }
      else
      if (fs_loc > 0) {
        c2r_bintree_r++; // ����������
        p_tmp = p_ii0->p_poi[3];
        if (!p_tmp) {    // ����� �� ����� ����� ����� �� �������� ��������                  
          v_v = 3;
          break;
        }
      }
    
      p_ii2  = p_ii1;	// �������� ������� ������
      p_ii1  = p_ii0;	// �������� ������� ������
      p_ii0  = p_tmp;	// �������� ������� ������

    }
  }

  P_BFL = p_ii0;	// ���������� ��������� �� ������ ���������� ���� �� ��������� ���������� �������

}

/*-----------------------------------------------------------*/
void create_new_rec() {

  p_new = (t_d_s *) calloc(1, sizeof(t_d_s));
  if (!p_new) {
    printf("�����稫��� �������᪠� ������. ���-���...\n");
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
// ������� ������ NEW ������������ P_BFL
// �������� ������ �������������� ���������

t_d_s *p_l;         
t_d_s *p_r;         
t_d_s *p_c;
t_d_s *p_prev;
t_d_s *p_x;
t_d_s *p_kkk;
int   i;
  
  p_kkk = P_BFL;
/*	����� ��� �������� ��������
  if (!n_rec) {
    return;
  }
*/
/**********************************************************************/
/* �������� ���������� �� ������ ������ */

  p_c = p_new;

  if (fs > 0) {
    p_l = p_kkk; p_r = p_l->p_poi[1]; // ������� ����� ������ �� p_kkk
    if (p_r) 
      p_r->p_poi[0] = p_c;            // ������� ������ � ���������
    else {
      p_fin = p_c; p_c->p_poi[1] = NULL; p_x_hi0 = p_c; p_x_hi1  = p_kkk; p_x_hi2 = p_ii1; fl_extreme =  1; 
    }
    p_l->p_poi[1] = p_c;              // ������� ������ � ���������
  }
  // fs == 0 - ������ ��� ���� �� ������
  else {
    p_r = p_kkk; p_l = p_r->p_poi[0]; // ������� ����� ����� �� p_kkk    
    if (p_l) 
      p_l->p_poi[1] = p_c;            // ������� ������ � ���������
    else {
      p_beg = p_c; p_c->p_poi[0] = NULL; p_x_hi0 = p_c; p_x_hi1 = p_kkk;  p_x_hi2 = p_ii1; fl_extreme = -1; 
    }
    p_r->p_poi[0] = p_c;              // ������� ������ � ���������
  }
  p_c->p_poi[0] = p_l;  p_c->p_poi[1] = p_r;

//old
/* ������ ������������� ������� ���������� */
  if ((fs ^ fs_1) < 0)                        // ����������� ���� �������������
    p_x = p_kkk; // p_c == p_new;
  else 
    p_x = p_c, p_c = p_kkk; 

  p_prev = p_ii1;
//  p_c = NULL;
  if (n_rec >= 8){                            // �������� �������� - �� ��������������
    if (!p_prev->p_poi[3]) {                      
      p_r = p_prev; p_l = p_x;                // �� ���� ������ �����
      // ����� p_c �� �������
    }
    else {
      if (!p_prev->p_poi[2]) {                      
        p_l = p_prev; p_r = p_x;                // �� ���� ������� �����
        // ����� p_c �� �������
      }
      else {
        p_c = NULL;
      }
    }
  }
  else {
    p_c = NULL;
  }

  if (p_c) {                            // ���� p_c �� ������� - �������� � ������������
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
  else {                               // ����� �������� ��� �����������
    p_kkk->p_poi[v_v] = p_new;
  }
  
  /* �������� */
  /* �������� ���� ���-�� ������� �������� ���������� ������ 2**n */  
  /* ��� ���������� ������� ��������� � ������� �����, ����������� � �������, ��������� ���-�� �������) */

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
    v_2n = v_2n + v_2n;		// v_2n     �ਭ����� ���祭�� 1, 2, 4, 8, 16, ....., 65536, ... 
    v_n++;                      // v_n      �ਭ����� ���祭�� 0, 1, 2, 3,  4, .....,    10, ... 
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
      fix_first_rec();  // ����� ���������� p_beg � p_fin ���������������� ������.
    }
    else {
      binary_finding(p_new);   /* return fs & P_BFL as GLOBAL */
      insert_new_rec();                                        
    }
    n_rec++;
  }

//  p_mid = bin_razbiv2(p_beg, p_fin); // �������� ���� �� ���������� p_beg � p_fin

  return 0;
}

/* ************************************************************* */
void del_rec(t_d_s *p_del) {
// ���������� �������� ������ p_del.
// �������� ������ �������������� ���������.
// ��������� �������� ������� ����� ��������� �� �� ��������� ������, � ���� �� �����.

// ��������� p_del �������� �� ����� ��� ���������, 
// ��������� P_poi, ��-�������� ��������� �� �������� ������.
// ��� ��� ���� �� �������� del_rec �� �����, 
// �� � ���� ����� �� �������� �� ��������� �������� ������.

t_d_s *p_lft;
t_d_s *p_rgt;

  p_lft = p_del->p_poi[0];   /* ������ p_lft ��������� �� ���������� ����� */
  p_rgt = p_del->p_poi[1];   /* ������ p_rfg ��������� �� ���������  ����� */

  n_rec--;
  if ((p_del != p_beg) && (p_del != p_fin)) {
    p_lft->p_poi[1] = p_rgt; /* ������ ��. � ����������� ��. ����� ��������� �� ���������  ��. */
    p_rgt->p_poi[0] = p_lft; /* �����  ��. �� ���������� ��. ����� ��������� �� ���������� ��. */
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
    printf("����������� ������������ ������. ����-����...\n");
    exit(1);
  }
  form_new_rec();

  n_rec++;

  if (n_rec == 1) {
    p_beg = p_new;
    p_beg->p_poi[0] = NULL;
  }
  else {
    // p_fin ������ ���������� ������, � p_new - �������
    p_fin->p_poi[1] = p_new;
    p_new->p_poi[0] = p_fin;
  }
  p_fin = p_new;
  p_fin->p_poi[1] = NULL;
}
