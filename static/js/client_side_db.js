// This file is used as an example for accessing a database with out a server
//
//      Not typically recommended since it can cause security issues and its a little finicky. However it can be done if this is the route we want to take.
//      Sauce: https://stackoverflow.com/questions/857670/how-to-connect-to-sql-server-database-from-javascript-in-the-browser

function dbConnect(){
    var connection = new ActiveXObject("ADODB.Connection") ;

    var connectionstring="Data Source=../../testData.db;Initial Catalog=<catalog>;User ID=<user>s;Password=<password>;Provider=SQLOLEDB";

    connection.Open(connectionstring);
    var rs = new ActiveXObject("ADODB.Recordset");

    rs.Open("SELECT * FROM table", connection);
    rs.MoveFirst
    while(!rs.eof)
    {
    document.write(rs.fields(1));
    rs.movenext;
    }

    rs.close;
    connection.close; 
}

function linkTest(){
    alert("Files linked")
}