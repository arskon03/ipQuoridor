# ipQuoridor
4th assignment

- Το πρόγραμμα κάνει compile μέσω του αρχείου Makefile, οπότε άπλα χρειάζεται να τρέξεις το command make.

- Τα άτομα τα οποία έφτιαξαν ομαδικώς το πρόγραμμα είναι:
    1. Αργύριος Λαζαρίδης           ΑΜ: 1115202100083
    2. Κωνσταντίνος Δημητρόπουλος   ΑΜ: 1115202100033

- Η συνάρτηση main δεν δέχεται ορίσματα

- Δεν χρησιμοποιείται τυχαία γεννήτρια αριθμών ούτε μαθηματική βιβλιοθήκη

- Η playmove, η legal move, οτι έχει να κάνει με το history, η undo, οι toLower/Upper και οι toVertex/Array υλοποιήθηκαν κυρίως από τον Δημητρόπουλο Κωνσταντίνο
- Η playwall, η genmove, showboard, η commands, όλο το pathfinder.c, η minimax, η execute, η boardsize, η clearboard και η connected υλοποιήθηκαν κυρίως από τον Αργύριο Λαζαρίδη

- Όμως, είναι σημαντικό να σημειωθεί ότι γενικώς μοιραζόμασταν ιδέες, καμμάτια κώδικα, κάναμε debugging μάζι όλων τον κώδικα και κάναμε τροποποιήσης στα κομματια του άλλου
 πχ. τα εναλλασόμενα depths υλοποιήθηκαν από τον Δ.Κ. και τα checks για άκυρα parameters στην playmove υλοποιήθηκαν από τον Α.Λ.

- Τα μόνα κομμάτια στα οποία ο καθένας δουλέψε αποκλειστικά μόνος του είναι με ελάχιστη:
    1. Το game history και η undo από ΚΔ
    2. Η pathfinder.c από ΑΛ

- Όλα τα υπόλοιπα αναπατύχθηκαν συνδυαστικά και από τους δύο

- Recommended board size : 7
 
- Sources:
    1. Breadth-first search: 
        - https://en.wikipedia.org/wiki/Breadth-first_search
        - https://www.youtube.com/watch?v=KiCBXu4P-2Y&ab_channel=WilliamFiset