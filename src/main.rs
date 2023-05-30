#![allow(non_snake_case)]
#![no_std]
#![no_main]

use core::panic::PanicInfo;

static MSG: &[u8] = b"Started!";

#[no_mangle]
pub extern "C" fn _start() -> ! {
    let buff = 0xb828000 as *mut u8;

    for (i, &byte) in MSG.iter().enumerate() {
        unsafe {
            *buff.offset(i as isize * 2) = byte;
            *buff.offset(i as isize * 2 + 1) = 0xb;
        }
    }

    loop {}
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
