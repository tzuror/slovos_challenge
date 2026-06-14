SECTIONS
{
    .primary : {
        __primary_start = .;
        *(.primary)
        __primary_end = .;
    }
    .mySegment2  : {
            __mySection2Start = .;
            *(.mySection2)
            __mySection2End = .;
    }
    . = . + 0x1000;
    .mySegment : {
        __mySectionStart = .;
        *(.mySection)
        __mySectionEnd = .;

    }

    .seseq : {
        __to_encrypt_start = .;
        *(.seseq)
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