
Aprasymas
---------

Directorijos struktura
----------------------

  README.txt - sis failas
  Main.java - Klase pateikianti duomenu transformaciju vartojimo
              intefeisa, kurioje yra "main" metodas.
  Data.java - abstrakti duomenu klase/implementacija
  *BWT.java - BWT klase/implementacija
  *MTF.java - MTF transformacijos klase/implementacija
  *HUFFMAN.java - HUFFMAN kodo generavimo klase/implementacija
  my/Node.java - HUFFMAN medzio mazgo klase
  my/Symbol.java - HUFFMAN kodavimo pagalbine struktura (klase)
  my/Transformation.java - Transformacijos interfeisas (bendras
                           visoms transformacijoms)


Programos kompiliavimas
-----------------------

javac Main.java


Programos paleidimas
--------------------

java -Xmx96m Main <[cd]> <input_file_name> <output_file_name>


Argumentu aprasymas
-------------------

-Xmx96m - Java virtualios masinos parametras, nurodantis maksimalu atminties kieki,
          skirta programos vykdymui.

<[cd]> - Programos parametras, "c" - "compress", "d" - "decompress", reikia nurodyti
         viena simboli.

<input_file_name> - Ieities duomenu failo pavadinimas.
<output_file_name> - Iseities duomenu failo pavadinimas.


Pavazdys
--------

javac Main.java
java -Xmx96m Main c data.in data.out
java -Xmx96m Main d data.out data.in.0
diff data.in data.in.0

Pastabos
--------

Pagal nutylejima, Javos virtuali masina iskiria 64MB atminties programos vykdymui.
Sito atminties kiekio neuztenka, todel reikia isskirti daugiau atminties.

"diff" - UNIX operacines sistemos programa, leidzianti matyti skirtumus tarp dvieju
         failu.
