"ui";
ui.layout(
    <frame>
        <vertical h="auto" align="center" margin="0 50">
            <text text="è¯·è¾.å.¥flag" textColor="black" textSize="18sp"/>
            <input id="flag" />
            <button id ="ok" w="auto" h="auto" text="ç¡®å®."  style="Widget.AppCompat.Button.Colored"/>
        </vertical>
        
    </frame>
);
ui.ok.on("click",()=>{
    var result=[0x2e,0xde,0x94,0xc2,0x41,0x8f,0xe3,0xfa,0xfb,0x10,0x4f,0x96,0x64,0xbf,0x2d,0xe3,0x96,0xf1,0x6c,0xa1,0x6b,0xb6,0x9a,0x94,0xfb,0x70,0x3f,0x4b,0x4b,0x7e,0x35,0xc7,0x10,0x90,0x57,0xcb];
    shell("chmod 777 /data/data/com.telegram.messenger/files/project/D0g3.jpeg",true);
    shell("/data/data/com.telegram.messenger/files/project/D0g3.jpeg"+" "+ui.flag.text(),true);
    if(files.isFile("/sdcard/1A.txt")){
        var data=files.readBytes("/sdcard/1A.txt");
        for(var i=0;i<data.length;i++){
            if((data[i]&0xff)!=result[i]){
                shell("rm -f /sdcard/1A.txt",true);
                toast("wrong");
                return 0;
            }           
        }
        toast("right");
        shell("rm -f /sdcard/1A.txt",true); 
    }
    else{
        toast("wrong");
    }    
})