white=0xffff
blue=0x001f
green=0x07e0
purple=0xf81f
red=0xf800
cyan=0x07ff
yellow=0xffe0
black=0x0000
gray=0x8410
darkgray=0x4208
lightgray=0xc618
boards=[
  "displayless","M5Stack","M5StackCore2","M5StickC","M5StickCPlus",
  "M5StickCPlus2","M5StackCoreInk","M5Paper","M5Tough","M5Station",
  "M5StackCoreS3","M5AtomS3","M5Dial","M5DinMeter","M5Cardputer",
  "M5AirQ","M5VAMeter","M5StackCoreS3SE","M5AtomS3R",
]

def hit_any_key
  M5.update
  return (BtnA.is_pressed? or BtnB.is_pressed? or (Touch.count>0 and Touch.was_clicked?(0)) )
end

#
while true do
    puts "Hello demo"
    board=M5.board
    if board>127 then board=0 end
    puts "Board: #{boards[board]}(#{board})"
    Display.clear

    if Speaker then
    Speaker.volume = 20
    Speaker.tone 1000,100,0
    Speaker.tone 2000,100,0,false
    end

    Display.set_text_size(1) if Display.dimension[0]<240
    Display.set_cursor(0,0)
    Display.puts "Board: #{boards[board]}(#{board})"
    Display.puts "Display: #{Display.dimension.inspect}"
    Display.puts "Touch: " + ((Touch.available?)? "OK":"NG")
    Display.puts

    cur=Display.get_cursor
    cur[0]+=12 if Display.dimension[0]>300
    Display.set_cursor(*cur)
    Display.set_text_size(4) if Display.dimension[0]>300
    [["H",white,red],["e",white,green],["l",white,blue],["l",white,purple],["o",white,cyan], [" ",white,black], 
    ["W",white,gray], ["o",white,darkgray], ["r",white,lightgray], ["l",white,white], ["d",white,black], ["!",white,red], ["\n",white,black]
    ].each do |c|
    Display.set_text_color(c[1],c[2])
    Display.print c[0]
    end

    Display.set_text_size(2);
    Display.set_text_color(0xffff)

    sleep 1
    Display.puts
    Display.puts "Hit any key"
    while !hit_any_key  do 
        sleep 0.1 
        return if Blink.req_reload?
    end
    Display.puts "!"

    xscale=2.0
    yscale=2.0
    xoffset=0
    yoffset=50
    case board 
        when 3,4,5 # M5StickC
        xscale=0.6
        yscale=0.6
        xoffset=0
        yoffset=80
        when 11 #atomS3
        xscale=0.8
        yscale=0.8
        yoffset=50
    end

    if board>0 then
        puts "Mexican Hat"
        Display.clear
        Display.puts "Mexican Hat"
        def dot(x,y,c)
            Display.draw_line(x.to_i,y.to_i,x.to_i,y.to_i,c)
        end

        d=Array.new(160,100)

        dr=3.141592/180
        (-30..30).each do |by|
            (-30..30).each do |bx|
                x=bx*6; y=by*6
                r=dr*Math.sqrt(x*x+y*y)
                z=100*Math.cos(r) - 30*Math.cos(3*r)
                sx=(80+x/3-y/6).to_i
                sy=(40-y/6-z/4).to_i
                if sx>=0 && x<160 then
                    if d[sx]>sy then
                        zz=((z+100)*0.035).to_i+1
                        if [1,2,5,7].include?(zz) then
                            dot(xscale*sx+xoffset,yscale*sy+yoffset,white)
                        elsif  [2,3].include?(zz) or zz>=6 then
                            dot(xscale*sx+xoffset,yscale*sy+yoffset,green)
                        elsif zz>=4 then
                            dot(xscale*sx+xoffset,yscale*sy+yoffset,purple)
                        end
                        d[sx]=sy
                    end
                end
            end
        end
        Display.puts "Hit any key"
        while !hit_any_key do 
        sleep 0.05 
        return if Blink.req_reload?
        end
        Display.puts "!"
        sleep 0.5
    end

    puts "Picture"

### picture load section
    mrubycpng=
        "\x89PNG\r\n\x1A\n\x00\x00\x00\rIHDR\x00\x00\x00\x19\x00\x00\x00\x19\b\x02\x00\x00\x00K\x8B\x124\x00\x00\x00\x06tRNS\x00\x80\x00\x00\x00\xFF\xF2BX\x8E\x00\x00\x00\tpHYs\x00\x00\v\x13\x00\x00\v\x13\x01\x00\x9A\x9C\x18\x00\x00\x03\x8CIDAT8\x8D\xB5T\xCFk\\U\x14>\xE7\xFEz\xEF\xCD\x9B\x99L&\xB1\xCE06M\x9345\x051\x16\n\x82?p%\xB5\x8B\xA8P\xFF\x00\x97\"\x05\x95n\xDC\xB8u\xE9F]\b\x82\xB8S\xD0\n\xD5\x8D\x05E\xA4U\xB0\xA5R\xC1\xC6\xD8Bc\xC4\x18\xD3\xC9\xFC\x9Ey\xF7\xDD\x1F\xC7\xC5\xB4\xC9\xCCt(\x04\xF4\xE3q\xE1\xBB\x87\xEF\xE3\xDC\xEF\xDDs\x01\xFE;\xE0\xBD[K2Z\t\x8Bc\n\x00\x00P62\xF6\xEC\x82o|b\xAB#%1\xC2\vL\x9C\xCDV\x8A\xC8g\x8C\xDA\x126A?X}\xC0\xCAC \x81\xC1c*\xBE\x96\xF4\xAE\xDB\xEE`\x95\r7\x89g\xB2\xA5\"\x8A\xC3iP\xB4B#\x85\xC4\x96t\x18\x12\x03\x80\xBC\xE73F\x02@\x87\xFB\re^\xCF\x96s\x8C\x0F\xCA\x87\xC8\x8B\xD1\xD4\xA9`r>U\x93\x96\xD7\x85\xE3\x04\vi\x10xl\n\x87\x80\x8B:d\x04)\xA3\xB5P;\xA0\f\xB2\n\x0F.\xA6\xCD1^\x8B\"z3\xAE<\xAC\xC3\xACc\xFD\x1E\xA7\xAD\xE8\xA7\xD6ct\xD0(I\xE8\x10\xD6B\xAD\x91\xFA\x92\nWm\xF2\xBF\xDBd\xC8+B\xF6^\xE6\xF0\xB2\xC9H\xBA\x13\xBA\xA0\xBD\xF4s\x9E\vB@\xB8\x19\xA4m\xE6\x06\x8F\xF2\x88\xCC\\6\xED\xBAw{\xD9O\x808f\xA3\xA1\xF0\xEEb=H\x9B\xCC\x03\xC0CF\xCE\xA4\x12@\x0EV\t\xE0\x10\x05\xB7@\xEFe\xFF7\xA5\xA7\xD3\xB5M2#F\x7F)\xB3\xCD\xADF_p|\xD2r\xE5q\xF0#\x0Fo\xE8\xF5\xEF\xDC\x9D\xC8\x86\xAEQ\t\xD5Gj\xEE\x18F}Z\x15\xEE\x96\xD2\x04P\xF0\xE2\xA0\x96U\xB2m\xF2\b\xD4\x97u\xC0\xBFe\xFE\xBC\xEA;\xBB\xF2\xD1+\x99E\xFE\xBE\x9C}\x86\xE5\xDB\xDC\xAF\x05\xDA\x03e\x88-$\xC1\rJ\xBE\xB2\xF5U\xEAy\x00\x01h\x80\xAE\xFAN\x8D\xEC\xA0\x96\x8Fx\xA5@\xE7}\xBD\xC0\x84\xCA0\a$\t\x8F\xE8p\x9B\xCC\xE7\xB6v\x81\x1A\xCFe&\x17U4\xAB\xC2\x92\x94\x97L+%\xBA\x9F\x17\x00x\x80o]3\x01Z\x96\xF1Q\x1D\xA0\xC7\x9F|\xE7S\xB7\xB3\x12\x17\x8F\xCBx\x9A\x89i&\xCA\\\x95\xB9\xBA\x98\xB6\x06\x85\xA33\xB4\x8Bs\xBDj\xD7\xB8w\xC5,\x00\x04\x80\x05\xE4\xD3\x9A\x1F\xB7\x19\x00\xB8\x11\xE8\xB2\x91\xBF\xA4\xDD\x11\xC9\x98\xBEvq\xD3'?\xF8\xF6I>QB\x99C\xFE\xB6\xD9\xAC\x91{\x92\xE5j\xDC}\x90\xFC\xF3\x8E\xD9\xBC\x8Fv<f1\xF8P\xCD?\xC1s}z\x8AO\x9E\xE4\x85}\xBB\xFC/@@\x81\xE3\x1F4\x85{\xC32vlFqT\x86+a\xF1\xDE\xFD\t&\xCE\xC4\xA5]\xCA9\xE2\xE3*7\xC5d\x81\x89\x02\x135o\x17E\xB4 \xA2\xDB\xDEV\x84\xEA\x90\x9F\x13!G\x9C\xE7a\x83\\\xDD\xDB#\"\xDA\xB9\xBB\xBE\x10\x16\xAF\x98N\x17|\x89\xCB\x86w<\xCF\xC4\xABq)D\xBE\xAC\xE2Ge\xBC\xEAz\xAF\xC4\xA5\xAA\xB7O\a\xF99\x11\xAE[\xFDr\xE6\xC0\xAF\xB6\xF7|T\xCC!/q\xB5,c@8\x1DM]J\xDB/ES\xE7u\xEDl\xB6\xFC \x93\xA6\x7F\xC6u\xA7\x7FL[\xD7L\xF7\xBA\xEDM\xA0\xE8\x92\xFB2\xD9\xF9,\xA9\xA6D%.c\xE4\x00\xB0\xE5\xCC\xCF\xA6{\x80\xCBsI\xF5\xB5l\xF9\xEB\xA4\xFET\x90\xFB>mf\x10[\xE4\xAF\x98\x8E&\xCF\t@\"\xFB\xC3\xE9\x04\xA8\xE9\xDD\x967\x8A\xB1\xDFl\xAFK~\xC3\xE9g\xC3\xC2\x86\xD7\xAB6)2\x91E\xF6E\xAF\xD6\"wB\xE5>\xEEm/\xC9\xE8\e\xDDlx\xE7\x00fxp\xD9\xB4\xF7\xF5?\x01\x00\xF2\x8C\x9FP\xD9}\xCB\xF6\x8B\x7F\x01/V\x8Fs\xBC\xFBA\xC6\x00\x00\x00\x00IEND\xAEB`\x82"

### end of picture load section

    Display.clear
    Display.puts "picture test"
    puts "Supported fonts: #{Font.names.inspect}"

    Display.draw_pngstr(mrubycpng,30,60*yscale)

    dimension=Display.dimension

    Display.draw_pngstr(mrubycpng,dimension[0]/2,dimension[1]/2)


    Display.puts "Hit any key to restart"
    while !hit_any_key do 
        sleep 0.05 
    end
end



