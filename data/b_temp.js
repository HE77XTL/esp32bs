function handleDate(temperatura, umidade, pressao) {
    console.log("temperatura: " + temperatura);
    console.log("umidade: " + umidade);
    console.log("pressao: " + pressao);
    if(temperatura > 30){
        console.log("打开空调");
    }
    if(umidade > 60){
        console.log("打开加湿器");
    }
    if(pressao < 800){
        console.log("打开灯光");
    }
}