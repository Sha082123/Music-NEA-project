use libmxgen::generate::paths::Paths;
use libmxgen::xsd::id::{Id, RootNodeType};
use libmxgen::xsd::Xsd;

const EXIT_SUCCESS: i32 = 0;
const EXIT_FAIL: i32 = 1;

macro_rules! must {
    ($result:expr) => {
        match $result {
            Err(e) => {
                eprintln!("{}:{} {:?}", file!(), line!(), e);
                std::process::exit(EXIT_FAIL);
            }
            Ok(o) => o,
        }
    };
}

fn main() {
    let paths = Paths::default();
    let mut xsd = must!(Xsd::load(&paths.xsd_3_0));
    let id = Id::new(RootNodeType::Element, "score-partwise".to_owned());
    let found = must!(xsd.find(&id)).to_owned();
    let entity = must!(xsd.remove(found.id()));
    println!("{:?}", &entity);
    std::process::exit(EXIT_SUCCESS);
}
