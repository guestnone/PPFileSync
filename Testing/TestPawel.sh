#!/bin/bash



pth="./"
echo $pth
mkdir $pth/InputPawel
mkdir $pth/OutputPawel


###                      DEST


#folder bez pokrycia w dest z plikiem

mkdir $pth/OutputPawel/folderbezpokrycia1_dest
touch $pth/OutputPawel/folderbezpokrycia1_dest/file1_folderbezpokrycia1_dest

#folder bez pokrycia w dest bez pliku

mkdir $pth/OutputPawel/folderbezpokrycia2_dest

#folder dest z pokryciem z plikami

mkdir $pth/OutputPawel/folderzpokryciem1_dest
touch $pth/OutputPawel/folderzpokryciem1_dest/plikold_forlderzpokryciem1_dest
touch $pth/OutputPawel/folderzpokryciem1_dest/plikbezpokrycia_folderzpokryciem1_dest

#touch $pth/OutputPawel/folderzpokryciem1_dest/plikzpokryciem_folderzpokryciem1_dest

#folder z pokryciem pusty dest

mkdir $pth/OutputPawel/folderzpokryciem2_dest

###                      SRC

### folder z pokryciem 1
mkdir $pth/InputPawel/folderzpokryciem1_dest

cp $pth/OutputPawel/folderzpokryciem1_dest/plikold_forlderzpokryciem1_dest $pth/InputPawel/folderzpokryciem1_dest/plikold_forlderzpokryciem1_dest

touch $pth/InputPawel/folderzpokryciem1_dest/plikzpokryciem_folderzpokryciem1_dest
cp $pth/InputPawel/folderzpokryciem1_dest/plikzpokryciem_folderzpokryciem1_dest $pth/OutputPawel/folderzpokryciem1_dest/plikzpokryciem_folderzpokryciem1_dest


### folder bez pokrycia 1

mkdir $pth/InputPawel/folderbezpokrycia1_src
touch $pth/InputPawel/folderbezpokrycia1_src/plik_folderbezpokrycia1_src

#folder z pokryciem2 
mkdir $pth/InputPawel/folderzpokryciem2_dest
touch $pth/InputPawel/folderzpokryciem2_dest/plik_folderzpokryciem2_dest

# odpal synchronizator
./../PPFileSync -s InputPawel -d OutputPawel -w 10 -v -R





























