SECTIONS
{
    .text : {
        __start_text = .;
        *(.text)
        __end_text = .;
    }
    .mySegment2  : {
            __mySection2Start = .;
            *(.mySection2)
            __mySection2End = .;
    }
    . = . + 0x1000; /* Align to the next 4KB boundary */
    .mySegment : {
        __mySectionStart = .;
        *(.mySection)
        __mySectionEnd = .;

    }

    .to_encrypt : {
        __to_encrypt_start = .;
        *(.to_encrypt)
        __to_encrypt_end = .;
    }

    /*.mySegment 0x19000 : {
        __mySectionStart = .;
        *(.mySection)
        __mySectionEnd = .;

    }
    .mySegment2 0x18000 : {
        __mySection2Start = .;
        *(.mySection2)
        __mySection2End = .;
    }*/


}