const char ROOT_PAGE[] = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
    <title>Chicken Gate</title>
    <style>
      div, input {
        padding: 5px;
        font-size: 1em;
        margin: 10px 0 10px 0;
      }
      input {
        width: 95%;
      }
      body {
        text-align: center;
        font-family: verdana;
      }
      button {
        border: 0;
        border-radius: 0.3rem;
        background-color: #1fa3ec;
        color: #fff;
        line-height: 2.4rem;
        font-size: 1.2rem;
        width: 100%;
      }
      button:disabled {
        background-color: grey !important;
      }
      #finishButton {
        margin-top: 10px;
        background-color: #ed2939;
      }
    </style>
    <script>
      const monthNames = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
      
      function addLeadingZero (number) {
        return (number < 10) ? `0${number}` : `${number}`; 
      };

      function addLeadingSpace (number) {
        return (number < 10) ? ` ${number}` : `${number}`; 
      };

      function updateTime () {
        const now = new Date();
        const dateString = `${monthNames[now.getMonth()]} ${addLeadingSpace(now.getDate())} ${now.getFullYear()}`;
        const timeString = `${addLeadingZero(now.getHours())}:${addLeadingZero(now.getMinutes())}:${addLeadingZero(now.getSeconds())}`;

        fetch(`/updateTime?date=${dateString}&time=${timeString}`)
          .then(function(result) {
            const button = document.getElementById('updateTimeButton');
            button.disabled = true;
            button.innerText = 'Updated';
          })
          .catch(function(error) {
            const status = document.getElementById('status');
            status.innerText = `${error.message}`;
          })
      }

      function setTimer (timer) {
        const time = document.getElementById(timer).value;
        if (!time) {
          return;
        }
        const now = new Date();
        const dateString = `${monthNames[now.getMonth()]} ${addLeadingSpace(now.getDate())} ${now.getFullYear()}`;
        const timeString = `${time}:00`;
        const status = document.getElementById('status');

        fetch(`/setTimer?date=${dateString}&time=${timeString}&mode=${timer}`)
          .then(function(result) {
            const button = document.getElementById(`${timer}Button`);
            button.disabled = true;
            button.innerText = 'Set';
          })
          .catch(function(error) {
            status.innerText = `${error.message}`;
          })
      }
      
      function finish () {
        fetch(`/done`)
          .then(function(result) {
            const button = document.getElementById(`finishButton`);
            button.disabled = true;
            button.innerText = 'Saved';
          })
          .catch(function(error) {
            status.innerText = `${error.message}`;
          })
      }

      function readVoltage () {
        fetch(`/readVoltage`)
          .then(function(response){
            return response.json();
          })
          .then(function(result) {
            const element = document.getElementById(`voltage`);
            element.innerText = result.voltage;
          })
          .catch(function(error) {
            status.innerText = `${error.message}`;
          })
      }

      window.onload = function(event) {
        readVoltage();
      };
    </script>
  </head>
  <body>
    <div style="text-align:left; display:inline-block; min-width:260px;">
      <h1>Chicken Gate</h1>

      <h2>Time</h2>
      <button onclick="updateTime()" id="updateTimeButton">Update time</button>
      <h2>Timers</h2>
      <input type="time" name="timer_one" id="timerOne" />
      <button onclick="setTimer(`timerOne`)" id="timerOneButton">Set Open Time</button>
      <br/>
      <input type="time" name="timer_two" id="timerTwo" />
      <button onclick="setTimer(`timerTwo`)" id="timerTwoButton">Set Close Time</button>
      <button onclick="finish()" id="finishButton">Finish</button>
      <div id="status" />
      <h2>Voltage</h2>
      <div id="voltage" />
    </div>
  </body>
</html>
)=====";
