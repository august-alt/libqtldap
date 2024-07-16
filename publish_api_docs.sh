#!/usr/bin/env bash

### Create clean documentation branch ###
git checkout -b documentation

### Perform update before install ###
sudo apt-get update

### Install qdoc and qt5base ###
sudo apt-get install qdoc-qt5
sudo apt-get install qtbase5-dev

### Setting environment variables ###
export BUILDDIR=`pwd` 
export QT_VERSION=5.15.13 
export QT_VER=5.15.13 
export QT_VERSION_TAG=51513 
export QT_INSTALL_DOCS="/usr/share/doc/qt5/"

### Generate autodocs for libqtldap library ###
qdoc config.qdocconf -I /usr/include/qt5 -I /usr/include/qt5/QtCore/

### Add generated autodocs to Git repo in the documentation branch ###
git config --global user.email "github-bot@basealt.ru"
git config --global user.name  "$GITHUB_WORKFLOW GitHub action"
git add -f docs
git commit -am "docs: add auto-generated developer documentation"

### Publish autodocs as GitHub pages ###
git push -f origin documentation:documentation || :

### Always pass ###
if [[ ${{ steps.push_documentation.outcome }} != 'success' ]]; then exit 0; fi

### Result URLs ###
REPO_OWNER=$(echo $GITHUB_REPOSITORY | cut -d '/' -f 1)
REPO_NAME=$(echo $GITHUB_REPOSITORY | cut -d '/' -f 2)
echo "Documentation on a github.io:  https://$REPO_OWNER.github.io/$REPO_NAME/docs"
echo ""
echo "Documentation branch: $GITHUB_SERVER_URL/$GITHUB_REPOSITORY/tree/documentation"