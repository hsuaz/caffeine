<?php

/**
 * User Object Class
 */
declare(encoding='UTF-8');
namespace Ferrox\System\Text;

/**
 * Implement this as a PHP Extension Module
 * @author IceWolf
 *
 */
class Romanizer {
  protected $_stripAccents = FALSE;

  static protected $map = array(
  //static protected $latinMap = array(
    0xC6 => 'AE',    0xD0  => 'TH',   0xD8  => 'O',    0xDE  => 'TH',
    0xDF => 'ss',    0xE6  => 'ae',   0xF0  => 'th',   0xF8  => 'o',
    0xFE => 'th',    0x110 => 'D',    0x111 => 'd',    0x110 => 'H',
    0x111 => 'h',    0x131 => 'i',    0x131 => 'i',    0x141 => 'l',
    0x142 => 'l',    0x152 => 'OE',   0x153 => 'oe',   0x17F => 's',
  //);
  //static protected $greekMap = array(
    0x391 => array(0 => 'A', 0x3A5 => 'AV'),
    0x393 => array(0 => 'G', 0x393 => 'NG'),
    0x395 => array(0 => "E", 0x3A5 => 'EV'),
    0x39C => array(0 => "M", 0x392 => 'B'),
    0x39D => array(0 => "N", 0x3A4 => 'D', 0x39A => 'G'),
    0x39F => array(0 => "O", 0x3A5 => 'OU'),
    0x392 => "V",    0x394 => "TH",   0x396 => "Z",    0x397 => "I",
    0x398 => "TH",   0x399 => "I",    0x39A => "K",    0x39B => "L",
    0x39E => "KS",   0x3A0 => "P",    0x3A1 => "R",    0x3A3 => "S",
    0x3A4 => "T",    0x3A5 => "Y",    0x3A6 => "PH",   0x3A7 => "KH",
    0x3A8 => "PS",   0x3A9 => "O",

    0x3B1 => array(0 => "a",  0x03C5 => 'av'),
    0x3B3 => array(0 => "g",  0x3B3 => 'ng'),
    0x3B5 => array(0 => "e",  0x3C5 => 'ev'),
    0x3BC => array(0 => "m",  0x3B2 => 'b'),
    0x3BD => array(0 => "n",  0x3C4 => 'd',  0x3BA => 'g'),
    0x3BF => array(0 => "o",  0x3C5 => 'ou'),
    0x3B2 => "v",    0x3B4 => "th",    0x3B6 => "z",    0x3B7 => "i",
    0x3B8 => "th",   0x3B9 => "i",     0x3BA => "k",    0x3BB => "l",
    0x3BE => "ks",   0x3C0 => "p",     0x3C1 => "r",    0x3C2 => "s",
    0x3C3 => "s",    0x3C4 => "t",     0x3C5 => "y",    0x3C6 => "ph",
    0x3C7 => "kh",   0x3C8 => "ps",    0x3C9 => "o",

  //);
  //static protected $cyrillicMap = array(
    0x435 => array(0 => 'e', 0x308 => 'yo'),
    0x438 => array(0 => 'i', 0x306 => 'ii'),
    0x443 => array(0 => 'u', 0x306 => 'w'),
    0x456 => array(0 => 'i', 0x308 => 'yi'),
    0x430 => 'a',    0x431 => 'b',    0x432 => 'v',    0x433 => 'g',
    0x434 => 'd',    0x436 => 'zh',   0x437 => 'z',    0x43A => 'k',
    0x43B => 'l',    0x43C => 'm',    0x43D => 'n',    0x43E => 'o',
    0x43F => 'p',    0x440 => 'r',    0x441 => 's',    0x442 => 't',
    0x444 => 'f',    0x445 => 'x',    0x446 => 'ts',   0x447 => 'ch',
    0x448 => 'sh',   0x449 => 'shch', 0x44A => 'y',    0x44B => 'i',
    0x44C => 'y',    0x44D => 'e',    0x44E => 'yu',   0x44F => 'ya',
    0x452 => 'dy',   0x454 => 'ye',   0x458 => 'j',    0x459 => 'ly',
    0x45A => 'ny',   0x45B => 'tch',  0x45F => 'dj',   0x491 => 'g',

  //);
  //static protected $koreanMap = array(
    // Initials:
    0x1100 => 'g',    0x1101 => 'kk',    0x1102 => 'n',     0x1103 => 'd',
    0x1104 => 'tt',   0x1105 => 'r',     0x1106 => 'm',     0x1107 => 'b',
    0x1108 => 'pp',   0x1109 => 's',     0x110A => 'ss',    0x110B => '-',
    0x110C => 'j',    0x110D => 'jj',    0x110E => 'ch',    0x110F => 'k',
    0x1110 => 't',    0x1111 => 'p',     0x1112 => 'h',     0x1113 => 'nk',
    // Initials Historic:
    0x1114 => 'nn',   0x1115 => 'nt',    0x1116 => 'np',    0x1117 => 'tk',
    0x1118 => 'rn',   0x1119 => 'rl',    0x111A => 'rh',    0x111B => 'rh',
    0x111C => 'mp',   0x111D => 'w',     0x111E => 'bk',    0x111F => 'bn',
    0x1120 => 'bd',   0x1121 => 'bs',    0x1122 => 'bsg',   0x1123 => 'bsd',
    0x1124 => 'bsb',  0x1125 => 'bss',   0x1126 => 'bsj',   0x1127 => 'bj',
    0x1128 => 'bch',  0x1129 => 'bt',    0x112A => 'bp',    0x112B => 'f',
    0x112C => 'ff',   0x112D => 'sg',    0x112E => 'sn',    0x112F => 'sd',
    0x1130 => 'sr',   0x1131 => 'sm',    0x1132 => 'sb',    0x1133 => 'sbg',
    0x1134 => 'sss',  0x1135 => 'sng',   0x1136 => 'sj',    0x1137 => 'sch',
    0x1138 => 'sk',   0x1139 => 'st',    0x113A => 'sp',    0x113B => 'sh',
    0x113C => 'zhy',  0x113D => 'zhyzhy',0x113E => 'zh',    0x113F => 'zhzh',
    0x1140 => 'z',    0x1141 => 'ngg',   0x1142 => 'ngd',   0x1143 => 'ngm',
    0x1144 => 'ngb',  0x1145 => 'ngs',   0x1146 => 'ngz',   0x1147 => 'ngng',
    0x1148 => 'ngj',  0x1149 => 'ngch',  0x114A => 'ngt',   0x114B => 'ngp',
    0x114C => 'ng',   0x114D => 'jng',   0x114E => 'qy',    0x114F => 'qyqy',
    0x1150 => 'q',    0x1151 => 'qq',    0x1152 => 'chk',   0x1153 => 'chh',
    0x1154 => 'xy',   0x1155 => 'x',     0x1156 => 'pb',    0x1157 => 'f',
    0x1158 => 'hh',   0x1159 => 'h',     0x115A => 'gd',    0x115B => 'ns',
    0x115C => 'nj',   0x115D => 'nh',    0x115E => 'dr',

    // Vowels
    0x1161 => 'a',     0x1162 => 'ae',    0x1163 => 'ya',     0x1164 => 'yae',
    0x1165 => 'eo',    0x1166 => 'e',     0x1167 => 'yeo',    0x1168 => 'ye',
    0x1169 => 'o',     0x116A => 'wa',    0x116B => 'wae',    0x116C => 'oe',
    0x116D => 'yo',    0x116E => 'u',     0x116F => 'weo',    0x1170 => 'we',
    0x1171 => 'wi',    0x1172 => 'yu',    0x1173 => 'eu',     0x1174 => 'ui',
    0x1175 => 'i',
    // Vowels: Obsolote
    0x1176 => 'ao',    0x1177 => 'au',     0x1178 => 'yao',   0x1179 => 'yayo',
    0x117A => 'eoo',   0x117B => 'eou',    0x117C => 'eoeu',  0x117D => 'yeoo',
    0x117E => 'yeou',  0x117F => 'oeo',    0x1180 => 'oe',    0x1181 => 'oye',
    0x1182 => 'oo',    0x1183 => 'ou',     0x1184 => 'yoya',  0x1185 => 'yoyae',
    0x1186 => 'yoyeo', 0x1187 => 'yoo',    0x1188 => 'yoi',   0x1189 => 'ua',
    0x118A => 'uae',   0x118B => 'ueoeu',  0x118C => 'uye',   0x118D => 'uu',
    0x118E => 'yua',   0x118F => 'yueo',   0x1190 => 'yue',   0x1191 => 'yuyeo',
    0x1192 => 'yuye',  0x1193 => 'yuuu',   0x1194 => 'yui',   0x1195 => 'euu',
    0x1196 => 'eueu',  0x1197 => 'yiu',    0x1198 => 'ia',    0x1199 => 'iya',
    0x119A => 'io',    0x119B => 'iu',     0x119C => 'ieu',   0x119D => 'ia',
    0x119E => 'a',     0x119F => 'aeo',    0x11A0 => 'au',    0x11A1 => 'ai',
    0x11A2 => 'aa',    0x11A3 => 'aeu',    0x11A4 => 'yau',   0x11A5 => 'yeoya',
    0x11A6 => 'oya',   0x11A7 => 'oyae',

    // Finals:
    0x11A8 => 'k',     0x11A9 => 'kk',    0x11AA => 'gs',    0x11AB => 'n',
    0x11AC => 'nj',    0x11AD => 'nh',    0x11AE => 'd',     0x11AF => 'l',
    0x11B0 => 'rk',    0x11B1 => 'rm',    0x11B2 => 'rb',    0x11B3 => 'rs',
    0x11B4 => 'rt',    0x11B5 => 'rp',    0x11B6 => 'rh',    0x11B7 => 'm',
    0x11B8 => 'b',     0x11B9 => 'bs',    0x11BA => 's',     0x11BB => 'ss',
    0x11BC => 'ng',    0x11BD => 'j',     0x11BE => 'ch',    0x11BF => 'k',
    0x11C0 => 't',     0x11C1 => 'p',     0x11C2 => 'h',

    // Finals Obsolete
    0x11C3 => 'kl',    0x11C4 => 'ksk',   0x11C5 => 'nk',    0x11C6 => 'nt',
    0x11C7 => 'ns',    0x11C8 => 'nz',    0x11C9 => 'nt',    0x11CA => 'tk',
    0x11CB => 'tl',    0x11CC => 'lks',   0x11CD => 'ln',    0x11CE => 'lt',
    0x11CF => 'lth',   0x11D0 => 'rl',    0x11D1 => 'lmk',   0x11D2 => 'lms',
    0x11D3 => 'lps',   0x11D4 => 'lph',   0x11D5 => 'lv',    0x11D6 => 'lss',
    0x11D7 => 'lz',    0x11D8 => 'lk',    0x11D9 => 'lh',    0x11DA => 'mk',
    0x11DB => 'ml',    0x11DC => 'mp',    0x11DD => 'ms',    0x11DE => 'mss',
    0x11DF => 'mz',    0x11E0 => 'mch',   0x11E1 => 'mh',    0x11E2 => 'w',
    0x11E3 => 'pl',    0x11E4 => 'pp',    0x11E5 => 'ph',    0x11E6 => 'v',
    0x11E7 => 'sk',    0x11E8 => 'st',    0x11E9 => 'sl',    0x11EA => 'sp',
    0x11EB => 'z',     0x11EC => 'hk',    0x11ED => 'hkk',   0x11EE => 'hh',
    0x11EF => 'hk',    0x11F0 => 'hs',    0x11F1 => 'hz',    0x11F2 => 'fp',
    0x11F3 => 'pp',    0x11F4 => 'f',     0x11F5 => 'hn',    0x11F6 => 'hl',
    0x11F7 => 'hm',    0x11F8 => 'hp',    0x11F9 => 'h',     0x11FA => 'pn',
    0x11FB => 'gp',    0x11FC => 'gch',   0x11FD => 'gk',    0x11FE => 'gh',
    0x11FF => 'nn',
    /*
    // Historical Jamo Extension A
    0xA960 => 'dm',    0xA961 => 'db',    0xA962 => 'ds',    0xA963 => 'dj',
    0xA964 => 'rg',    0xA965 => 'rgg',   0xA966 => 'rd',    0xA967 => 'rdd',
    0xA968 => 'rm',    0xA969 => 'rb',    0xA96A => 'rbb',   0xA96B => 'rf',
    0xA96C => 'rs',    0xA96D => 'rj',    0xA96E => 'rk',    0xA96F => 'mg',
    0xA970 => 'md',    0xA971 => 'ms',    0xA972 => 'bst',   0xA973 => 'bk',
    0xA974 => 'bh',    0xA975 => 'ssb',   0xA976 => 'ngr',   0xA977 => 'ngh',
    0xA978 => 'jjh',   0xA979 => 'tt',    0xA97A => 'ph',    0xA97B => 'hs',
    0xA97C => 'hh',
    */
    // Historical Jamo Extention B D7B0-D7FB not listed

   //);
  //static protected $kanaMap = array(


  );


  public function romanize($string) {
    $utf16 = mb_convert_encoding($string, 'UTF-16', 'UTF-8');
    // This will strip accents for now
    $len = strlen($utf16);
    $output = array();
    for ($i = 0; $i < $len; $i += 2) {
      $key = (ord($utf16[$i]) << 8) + ord($utf16[$i+1]);

      switch (TRUE) {
        case ($key < 0x80) :
          $output[] = chr($key);
          break;

        case (array_key_exists($key, self::$map)) :
          // Straight Map
          if (is_string(self::$map[$key])) {
            $output[] = self::$map[$key];
            continue;
          }

          // Look Ahead
          if (($i + 2) < $len) {
            $lookAhead = (ord($utf16[$i+2]) << 8) + ord($utf16[$i+3]);
            if (array_key_exists($lookAhead, self::$map[$key])) {
              $output[] = self::$map[$key][$lookAhead];
              $i+=2;
              continue;
            }
          }
          // DEFAULT
          $output[] = self::$map[$key][0];
          break;

        case (!$this->_stripAccents) :
          if ($key >= 0300 && $key <= 0x361) {
            $output[] = mb_convert_encoding(chr($key>>8).chr($key&0xFF), 'UTF-8', 'UTF-16');
          }
          break;
        // NO MAP
      }
    }
    $output = implode('', $output);
    return $output;
  }
}