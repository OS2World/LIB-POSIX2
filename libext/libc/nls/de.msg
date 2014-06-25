$set 1
$ EPERM
1 Operation nicht erlaubt
$ ENOENT
2 Datei oder Verzeichnis nicht gefunden
$ ESRCH
3 Prozess nicht gefunden
$ EINTR
4 Interrupt innerhalb eines Systemaufrufs
$ EIO
5 Ein/Ausgabefehler
$ ENXIO
6 Geraet ist nicht konfiguriert
$ E2BIG
7 Argumentliste ist zu lang
$ ENOEXEC
8 Die Datei hat kein bekanntes ausfuehrbares Format
$ EBADF
9 Ungueltiger Dateideskriptor
$ ECHILD
10 Kein Kindprozess
$ EDEADLK
11 Ein Deadlock wurde vermieden
$ ENOMEM
12 Kann nicht genug Speicher belegen
$ EACCES
13 Zugriff verboten
$ EFAULT
14 Ungueltige Adresse
$ ENOTBLK
15 Es wird ein Blockgeraet benoetigt
$ EBUSY
16 Das Geraet ist belegt
$ EEXIST
17 Datei existiert bereits
$ EXDEV
18 Link zwischen verschiedenen Geraeten
$ ENODEV
19 Die Operation wird von diesem Geraet nicht unterstuetzt
$ ENOTDIR
20 Kein Verzeichnis
$ EISDIR
21 Ist ein Verzeichnis
$ EINVAL
22 Ungueltiges Argument
$ ENFILE
23 Zu viele offene Dateien im gesamten System
$ EMFILE
24 Zu viele offene Dateien
$ ENOTTY
25 Ungueltiger Ioctl fuer dieses Geraet
$ ETXTBSY
26 Ausfuehrbare Datei wird benutzt
$ EFBIG
27 Datei zu gross
$ ENOSPC
28 Kein Platz mehr auf dem Geraet
$ ESPIPE
29 Ungueltige Positionierung
$ EROFS
30 Dateisystem ist schreibgeschuetzt
$ EMLINK
31 Zu viele Links
$ EPIPE
32 Unterbrochene pipe
$ EDOM
33 Numerisches Argument ausserhalb des Wertebereichs
$ ERANGE
34 Ergebnis zu gross
$ EAGAIN, EWOULDBLOCK
35 Resource voruebergehend nicht verfuegbar
$ EINPROGRESS
36 Operation wird jetzt fortgesetzt
$ EALREADY
37 Operation wird bereits ausgefuehrt
$ ENOTSOCK
38 Deskriptor ist kein Socket
$ EDESTADDRREQ
39 Zieladresse benoetigt
$ EMSGSIZE
40 Nachricht zu lang
$ EPROTOTYPE
41 Ungueltiger Protokolltyp fuer diesen Socket
$ ENOPROTOOPT
42 Protokoll nicht verfuegbar
$ EPROTONOSUPPORT
43 Protokoll nicht unterstuetzt
$ ESOCKTNOSUPPORT
44 Sockettyp nicht unterstuetzt
$ EOPNOTSUPP
45 Operation nicht unterstuetzt
$ EPFNOSUPPORT
46 Protokollfamilie nicht unterstuetzt
$ EAFNOSUPPORT
47 Addressart wird von der Protokollfamilie nicht unterstuetzt
$ EADDRINUSE
48 Addresse wird bereits benutzt
$ EADDRNOTAVAIL
49 Kann angeforderte Adresse nicht belegen
$ ENETDOWN
50 Netzwerk nicht verfuegbar
$ ENETUNREACH
51 Netzwerk nicht erreichbar
$ ENETRESET
52 Netzwerk hat Verbindung mit Reset abgebrochen
$ ECONNABORTED
53 Software verursachte einen Verbindungsabbruch
$ ECONNRESET
54 Verbindung wurde von der Gegenstelle geschlossen
$ ENOBUFS
55 Keine Buffer verfuegbar
$ EISCONN
56 Socket ist schon verbunden
$ ENOTCONN
57 Socket ist nicht verbunden
$ ESHUTDOWN
58 Kann nach einem Socket-Shutdown nicht mehr senden
$ ETOOMANYREFS
59 Zu viele Referenzen, kann nicht verbinden
$ ETIMEDOUT
60 Verbindungsabbruch durch Zeitueberschreitung
$ ECONNREFUSED
61 Verbindung wurde abgelehnt
$ ELOOP
62 Zu viele symbolische Links (zirkulaer?)
$ ENAMETOOLONG
63 Dateiname zu lang
$ EHOSTDOWN
64 Host nicht verfuegbar
$ EHOSTUNREACH
65 Keine Route zum Host
$ ENOTEMPTY
66 Verzeichnis ist nicht leer
$ EPROCLIM
67 Zu viele Prozesse
$ EUSERS
68 Zu viele Benutzer
$ EDQUOT
69 Plattenplatzlimit erschoepft
$ ESTALE
70 Verwaister NFS-Dateideskriptor
$ EREMOTE
71 Zu viele Fernverweise in diesem Zugriff
$ EBADRPC
72 RPC-Struktur ist ungueltig
$ ERPCMISMATCH
73 RPC-Version stimmt nicht
$ EPROGUNAVAIL
74 RPC-Programm nicht verfuegbar
$ EPROGMISMATCH
75 Falsche Programmversion
$ EPROCUNAVAIL
76 Falsche Prozedur fuer dieses Programm
$ ENOLCK
77 Keine Dateisperren verfuegbar
$ ENOSYS
78 Funktion nicht implementiert
$ EFTYPE
79 Ungueltiger Dateityp oder Dateiformat
$set 2
$ SIGHUP
1 Verbindungsende
$ SIGINT
2 Unterbrechung
$ SIGQUIT
3 Programmende
$ SIGILL
4 Ungueltiger Maschinenbefehl
$ SIGTRAP
5 Trace/BPT trap
$ SIGABRT
6 Abort trap
$ SIGEMT
7 EMT trap
$ SIGFPE
8 Fliesskommafehler
$ SIGKILL
9 Killed
$ SIGBUS
10 Bus Fehler
$ SIGSEGV
11 Illegaler Speicherzugriff
$ SIGSYS
12 Ungueltiger Systemaufruf
$ SIGPIPE
13 Unterbrochene Pipe
$ SIGALRM
14 Wecker
$ SIGTERM
15 Beendet
$ SIGURG
16 Dringende Ein/Ausgabeanforderung
$ SIGSTOP
17 Gestoppt (signal)
$ SIGTSTP
18 Gestoppt
$ SIGCONT
19 Fortgesetzt
$ SIGCHLD
20 Kindprozess beendet
$ SIGTTIN
21 Gestoppt (Eingabe)
$ SIGTTOU
22 Gestoppt (Ausgabe)
$ SIGIO
23 Ein/Ausgabe ist moeglich
$ SIGXCPU
24 CPU-Zeitlimit erschoepft
$ SIGXFSZ
25 Dateigroesse hat das Limit erreicht
$ SIGVTALRM
26 Virtueller Wecker abgelaufen
$ SIGPROF
27 Profil-Wecker abgelaufen
$ SIGWINCH
28 Fenstergroesse hat sich geaendert
$ SIGINFO
29 Informationsanforderung
$ SIGUSR1
30 Benutzerdefiniertes Signal 1
$ SIGUSR2
31 Benutzerdefiniertes Signal 2
