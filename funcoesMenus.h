#ifndef FUNCOESMENUS_H_INCLUDED
#define FUNCOESMENUS_H_INCLUDED

char menu (int quantLaboratorios, int quantPostosTrabalho, int quantEquipamentos, float percEquipAvariados);
int subMenuLaboratorios(void);
int subMenuEquipamentos(void);
int subMenuAvarias(void);
int menuAlterarLaboratorio (void);
int menuAlterarEquipamento (void);

#endif // FUNCOESMENUS_H_INCLUDED
