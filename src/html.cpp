
#include <ESP8266WebServer.h>
#include "funcs.h"

void html(ESP8266WebServer* server){
char* page = R"(
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EspUI</title>
    <!-- <link rel="stylesheet" href="style.css">
    <script src="script.js" defer></script> -->
    <style>
        body {
    font-family: sans-serif;
    margin: 0;
    display: flex;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
    background: #222;
}

.container {
    max-width: 1200px;
    width: 90%;
    margin: 0 auto;
    border: 1px solid #fafafa;
    border-radius: 10px;
    display: flex;
    flex-direction: column;
    gap: 20px;
    padding: 10px;
}

.pins,
.terminal {
    width: 100%;
    background-color: #D9D9D9;
    border-radius: 10px;
    padding: 20px;
}

.terminal input {
    width: 100%;
    margin: 10px 0;
    background-color: #222;
    border: 1px solid #f5f5f5;
    border-radius: 5px;
    padding: 10px;
    color: #fafafa;
    font-size: 16px;
}

.pin {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-bottom: 10px;
}

.pin .name {
    width: 40%;
    padding: 10px 15px;
    background: #fafafa;
    border-radius: 5px;
    font-weight: bold;
}

.LOW .name {
    background-color: rgb(250, 61, 61);
}

.HIGH .name {
    background-color: rgb(61, 250, 92);
    color: #222;
}

.btn {
    padding: 10px 15px;
    background-color: #fafafa;
    border-radius: 5px;
    text-align: center;
    cursor: pointer;
}

.input_or_output {
    display: flex;
    align-items: center;
    gap: 10px;
}

.txt{
    height: 500px;
    display: flex;
    flex-direction: column;
    justify-content: start;
    overflow-y: scroll;
}
::-webkit-scrollbar{
    background-color: #222;
    border-radius: 5px;
}
::-webkit-scrollbar-track{
    border-radius: 5px;
}

::-webkit-scrollbar-track-piece{
    background-color: #222;
    border-radius: 5px;
}
::-webkit-scrollbar-thumb{
    background-color: #bababa;
    border-radius: 5px;
}

.txt-console{
    background-color: #222;
    padding: 10px;
    border-radius: 5px;
    color: green;
    margin: 1px;
    display: flex;
    justify-content: start;
    flex-direction: row-reverse;
    gap: 5px;
}

@media (min-width: 768px) {
    .container {
        flex-direction: row;
        gap: 30px;
    }

    .pins {
        width: 25%;
    }

    .terminal {
        width: 75%;
    }

    .pin .name {
        width: 20%;
    }
}

    </style>
</head>
<body>
    <div class="container">
        <div class="pins">
            <!-- Vai Ser Renderizado Pelo JS-->
        </div>
        <div class="terminal">
            <div class="txt"></div> 
            <input type="text" placeholder="Envie Menssagem">
            <div class="btn clearTerminal">Limpa Terminal</div>
        </div>
    </div>
    <script>
        const pins = [
["LED",2],
["D0", 16],
["D1", 5],
["D2", 4],
["D3", 0],
["D4", 2],
["D5", 14],
["D6", 12],
["D7", 13],
["D8", 15],
["RX", 3], 
["TX", 1]
]

var terminalText = []

const url = location.href

pins.forEach((e)=>{
    let pin = document.createElement('div')
        pin.classList.add("pin")
        pin.classList.add("LOW")

        let name = document.createElement('div')
        name.classList.add('name')
        name.textContent = e[0]

        let input_or_output = document.createElement('div')
        input_or_output.classList.add("input_or_output")
        input_or_output.classList.add("btn")
        input_or_output.textContent = "OUTPUT"
        input_or_output.style.background = "yellow"

        let value = document.createElement('div')
        value.classList.add('value')
        value.textContent = e[1]
        value.style.display = 'none'

        let btn = document.createElement('div')
        btn.classList.add("btn")
        btn.id = "btn"
        btn.textContent = "Ligar"
        
        pinEvent = e
        btn.addEventListener("click",(e)=>{
          

            if(e.target.textContent != "Ligar"){
                e.target.textContent = "Ligar"
                e.target.style.background = "white"
                e.target.parentNode.classList.remove("HIGH")
                e.target.parentNode.classList.add("LOW")

                
            }else{
                e.target.textContent = "Desligar"
                e.target.style.background = "red"
                e.target.parentNode.classList.remove("LOW")
                e.target.parentNode.classList.add("HIGH")
            }
            
        })  

        input_or_output.addEventListener('click',(e)=>{
            
            if(e.target.textContent == "OUTPUT"){
                e.target.textContent = "INPUT"
                e.target.style.background = "white"
            }else{
                e.target.textContent = "OUTPUT" 
                e.target.style.background = "yellow"
            }
        })

        pin.append(name,input_or_output,btn,value)


        document.querySelector(".pins").append(pin)
})

    document.querySelectorAll('.pin').forEach((e)=>{
        let btn = e.childNodes[2]

        btn.addEventListener('click',(e)=>{
            let pinName = e.target.parentNode.querySelector(".name").textContent
            let state = e.target.parentNode.querySelector("#btn").textContent === "Ligar" ? "HIGH" : "LOW";
            let output = e.target.parentNode.querySelector(".input_or_output").textContent
            let value = e.target.parentNode.querySelector(".value").textContent

            const data = {
                pinName,state,output,value
            }



            fetch(url+"pins",{
                method:"POST",
                headers:{"Content-Type":"application/json"},
                body:JSON.stringify(data)
            }).then(res=>res.text()).then(ev=> {

                document.querySelector('.txt').innerHTML = ''

                terminalText.push(ev);

                terminalText.forEach((e)=>{
                    let html = document.querySelector(".txt")
                    let newText = document.createElement("p")

                    let prefix = document.createElement("div")
                    prefix.textContent = "Terminal >"
                    prefix.style.color = "#26E7F3"

                    newText.textContent = e
                    newText.classList.add('txt-console')
                    
                    html.append(newText)
                    newText.append(prefix)

                })

            }).catch(err=>console.log("Error => " + err))
        })
    })

    document.querySelector(".clearTerminal").addEventListener('click',()=>{
        terminalText = []
        document.querySelector('.txt').innerHTML = ''
    })

    </script>
</body>
</html>
    )";

    server->send(200,"text/html",page);
}