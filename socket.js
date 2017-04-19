/**
 * Created by Netapp on 2017-04-01.
 */

//io = server , socket = client(Android)
module.exports = function(io) {
    io.on('connection', function(socket) {
        console.log('Socket initiated!');

        //Android starts Game(First Connection).
        io.emit('App start', 'Initialize Korean Chess Game');
        //socket.emit('App start', 'Initialize Korean Chess Game');

        //Android send request & server send response
        socket.on('App request', function(pos) {
            console.log('Client Position: ', pos);
            //This part should be changed to graph
            var next = JSON.parse(pos);
            console.log("Received Before Position: " + next.B_Pos + "\n" +
                "Received After Position: " + next.A_Pos);

            var B_Xpos = Math.floor(parseInt(next.B_Pos) / 10);
            var B_Ypos = parseInt(next.B_Pos) % 10;
            var A_Xpos = Math.floor(parseInt(next.A_Pos) / 10);
            var A_Ypos = parseInt(next.A_Pos) % 10;
            console.log("B_xPos: " + B_Xpos + ", B_yPos: " + B_Ypos);
            console.log("A_xPos: " + A_Xpos + ", A_yPos: " + A_Ypos);

            var nextPos = {
                "B_Pos" : parseInt(next.B_Pos) + 1,
                "A_Pos" : parseInt(next.A_Pos) + 1,
                "Unit" : "O"
            };
            //Make JSON data to response to client
            var nextJson = JSON.stringify(nextPos);
            io.emit('App response', nextJson);
            console.log("JSON Data Transmit: " + nextJson);
        });
        socket.on('App disconnect', function () {
           socket.disconnect();
           console.log('Disconnect from user');
        });
    });
};
//http://amritb.github.io/socketio-client-tool/#