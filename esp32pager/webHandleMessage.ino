void webEnterMSG() {
        String form = "<meta name=viewport content='width=200'><style>input, textarea {max-width:100%}</style><p><center><form action='msg2'><p></center>Previous MSG FROM: "+FROM+"<br>TO: <input type='text' name='TO' size=15 autofocus><br>Message: <input type='text' name='MSG1' size=75><center><br><br> <input type='submit' value='Submit'></form></center>";
        server.send(200, "text/html", form);// And as regular external functions:
      }

void webProcessMSG() {
        server.send(200,"text/html","<meta name=viewport content='width=200'> <meta HTTP-EQUIV=\"refresh\" CONTENT=\"20;http://192.168.4.1/\">Sent...");
        TO = server.arg("TO");
        TO.toUpperCase();
        MSG1 = server.arg("MSG1");
        sendMSG();
      }
