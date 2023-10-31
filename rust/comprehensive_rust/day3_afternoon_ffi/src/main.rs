// TODO: remove this when you're done with your implementation.
#![allow(unused_imports, unused_variables, dead_code)]

use std::os::unix::ffi::OsStrExt;
use std::os::unix::ffi::OsStringExt;

mod ffi {
    use std::os::raw::{c_char, c_int};
    use std::os::raw::{c_long, c_ulong, c_ushort, c_uchar};

    // Opaque type. See https://doc.rust-lang.org/nomicon/ffi.html.
    #[repr(C)]
    pub struct DIR {
        _data: [u8; 0],
        _marker: core::marker::PhantomData<(*mut u8, core::marker::PhantomPinned)>,
    }

    #[repr(C)]
    pub struct dirent {
        pub d_ino: c_ulong,
        pub d_off: c_long,
        pub d_reclen: c_ushort,
        pub d_type: c_uchar,
        pub d_name: [c_char; 256],
    }

    extern "C" {
        pub fn opendir(s: *const c_char) -> *mut DIR;
        pub fn readdir(s: *mut DIR) -> *const dirent;
        pub fn closedir(s: *mut DIR) -> c_int;
    }
}

use std::ffi::{CStr, CString, OsStr, OsString};

#[derive(Debug)]
struct DirectoryIterator {
    path: CString,
    dir: *mut ffi::DIR,
}

impl DirectoryIterator {
    fn new(path: &str) -> Result<DirectoryIterator, String> {

        // NDM : convertir le path en un *const c_char :
        let c_path = CString::new(path).map_err(|err| String::from("BOUM !"))?;
        unsafe {
            let c_dir = ffi::opendir(c_path.as_ptr());
            let ite = DirectoryIterator{ path: c_path, dir: c_dir };
            Ok(ite)
        }
    }
}

impl Iterator for DirectoryIterator {
    type Item = OsString;
    fn next(&mut self) -> Option<OsString> {
        unsafe {

            // je me retrouve à faire tout un tas de conversions, à mon avis je m'y prends mal :

            // [i8; 256]
            let dirent = ffi::readdir(self.dir).as_ref()?;

            // &[i8]
            let i8slice = dirent.d_name.as_slice();

            // &[u8]
            let u8slice : &[u8] = std::slice::from_raw_parts(i8slice.as_ptr() as *const u8, i8slice.len());

            // &CStr (nécessaire pour ignorer le reste de la string après le premier octet nul) :
            let c_str = CStr::from_bytes_until_nul(u8slice).unwrap();

            // &OsStr
            let os_str = OsStr::new(c_str.to_str().unwrap());

            // &OsString
            let os_string = os_str.to_os_string();

            Some(os_string)
        }
    }
}

impl Drop for DirectoryIterator {
    fn drop(&mut self) {
        unsafe {
            let retcode = ffi::closedir(self.dir);
            if retcode != 0 {
                panic!("non-zero closedir retcode");
            }
        }
    }
}

fn main() -> Result<(), String> {
    let iter = DirectoryIterator::new(".")?;
    println!("files: {:#?}", iter.collect::<Vec<_>>());
    Ok(())
}
