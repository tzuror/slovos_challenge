SECTIONS
{
    .text : {
        __start.text = .;
        *(.text)
        __end.text = .;
    }

    .mySegment 0x19000 : {
        __mySectionStart = .;
        *(.mySection)
        __mySectionEnd = .;

    }
    .mySegment2 0x18000 : {
        __mySection2Start = .;
        *(.mySection2)
        __mySection2End = .;
    }

}