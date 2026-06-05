# Documentação do código (`project/`)

Este repositório contém o jogo desenvolvido no âmbito de LCOM: lógica de **arena**, **monstros**, **torres**, **jogador**, **UI** e integração com o **terminal** (lab5).

## Como gerar a documentação

Na pasta `project/`:

```bash
cd /home/greedisland/00MyGit/01dev/school/lcom/MINIX-LCOM/shared/grupo_2leic13_2/project
doxygen Doxyfile
```

Abrir o índice HTML no navegador:

```bash
xdg-open docs/doxygen/html/index.html
```

Requisito: pacote `doxygen` instalado (`sudo apt install doxygen` em Debian/Ubuntu).

Para listar avisos de símbolos não documentados, no `Doxyfile` pode voltar a definir `WARN_IF_UNDOCUMENTED = YES` após completar comentários em ficheiros `.c` restantes.

## Âmbito

- Incluído: ficheiros em `include/` e `src/`.
- Excluído: `resources/` (assets XPM em cabeçalhos), `tests/`, e `timerTest.c`.
