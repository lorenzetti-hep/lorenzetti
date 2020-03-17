
git filter-branch -f --env-filter '
WRONG_EMAIL="rancher@zeus.lps.ufrj.br"
NEW_NAME="jodafons"
NEW_EMAIL="jodafons@lps.ufrj.br"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags




git filter-branch -f --env-filter '
WRONG_EMAIL="joao@gitlab.laspi.ufrj.br"
NEW_NAME="jodafons"
NEW_EMAIL="jodafons@lps.ufrj.br"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags









git filter-branch -f --env-filter '
WRONG_EMAIL="rancher@zeus.lps.ufrj.br"
NEW_NAME="jodafons"
NEW_EMAIL="jodafons@lps.ufrj.br"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags



git filter-branch -f --env-filter '
WRONG_EMAIL="jodafons@cern.ch"
NEW_NAME="jodafons"
NEW_EMAIL="jodafons@lps.ufrj.br"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags


git filter-branch -f --env-filter '
WRONG_EMAIL="joao.victor.da.fonseca.pinto@cern.ch"
NEW_NAME="jodafons"
NEW_EMAIL="jodafons@lps.ufrj.br"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags





git filter-branch -f --env-filter '
WRONG_EMAIL="jodafons@lps.ufrj.br"
NEW_NAME="jodafons"
NEW_EMAIL="jodafons@lps.ufrj.br"

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags

