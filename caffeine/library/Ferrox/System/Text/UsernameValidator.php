<?php
declare(encoding='UTF-8');
namespace Ferrox\System\Text;

  /**
   * Naming Conventions:
   * Cannot start name with a symbol
   * Cannot have consecutive spaces
   *
   * Language Coverage:
   * SYMBOL : -
   * ENGLISH   : ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789
   *                                        DBLACUTE                          OVERBAR  COMMA
   *                    RING    ACUTE              CIRCUMFLEX  HACEK        OVERDOT  BREVE
   *                      UMLAUT              GRAVE     CEDILLA         OGORNEKAR TILDE  BAR
   * ALL       : æðłøœßþåůäëïöüÿáćéíĺńóŕśúýźűőàèìòùâêîôûçģķļņŗşčďěľňřšťžąęįųċėġżāēīūãõñăğșțđħı
   * EU-ZONE
   * CZECH     :        ů       á éí  ó  úý                    čďě ňřšťž
   * DANISH    : æ  øœ  å       á éí  ó  ú
   * DUTCH     :-
   * ESTONIAN  :          ä  öü                                     š ž              õ
   * FINNISH   :        å ä  ö                                      š ž
   * FRENCH    : æ   œ    äëïöü   é           àè  ùâêîôûç                             ñ
   * GERMAN    :  ä   ß    ë öü
   * HUNGARIAN :             öü á éí  ó  ú  űő
   * IRISH     :                á éí  ó  ú
   * LATVIAN   :                                       ģķļņŗ č     š ž        āēīū õ
   * LITHUANIAN:                                             č     š žąęįų ė     ū
   * MALTESE   :                              àèìòù                         ċ ġż            ħ
   * POLISH    :   ł             ć   ńó ś  ź                            ąę     ż
   * PORTUGUESE:              ü á éí  ó  ú    à    âê ô ç                           ãõ
   * ROMANIAN  :                                   â î                                 ă șț
   * SLOVAK    :          ä     á éíĺ óŕ úý           ô        čď ľ  šťž
   * SLOVENE   :                 ć                             č     š ž                   đ
   * SPANISH   :              ü á éí  ó  ú                                            ñ
   * SWEDISH   :        å ä  öü   é        à            ç
   *
   * NON-EU
   * NORWEGIAN : æ      å         é   ó        è ò âê ô
   * ICELANDIC : æ     þ     ö  á éí  ó  úý
   * TURKISH   :             öü                         ç     ş                         ğ   ı
   * WELSH     :                                   âêîôû
   * MONTENEGRO:                 ć      ś  ź                   č     š ž
   * SERBIRAN(1:                 ć                             č     š ž                   đ
   *
   * NON-LATIN
   * GREEK     :  αβγδεζηθικλμνξοπρσςτυφχψω άέίόήύώ
   * GREEK     :  αβγδεζηθικλμνξοπρσςτυφχψω άέίόήύώ
   *
   * CYRILLIC
   * Cyrillic  : абвгґдђеєжѕзиіјклљмнњопрстћуфхцчџшщъыьэюя ёїӂйўѓќѐѝ
   * RUSSIAN   : абвг д е ж зи кл мн  опрст уфхцч шщъыьэюя ё  й
   * MACEDONIAN: абвг д е жѕзи јклљмнњопрст уфхцчџш             ѓќѐѝ
   * SERBIAN(2): абвг дђе ж зи јклљмнњопрстћуфхцчџ
   * UKRAINIAN : абвгґд еєж зиі кл мн опрст уфхцч шщ  ь юя  ї й
   * BELARUSS  : абвг д е ж з і кл мн опрст уфхцч ш  ыьэюя    йў
   * MOLDOVAN* : абвг д е ж зи  кл мн опрст уфхцч ш  ыьэюя   ӂй
   *
   * Latin Base Characters: a-z0-9æðłøœßþđħı
   * Latin Accented       : åůäëïöüÿáćéíĺńóŕśúýźűőàèìòùâêîôûçģķļņŗşčďěľňřšťžąęįųċėġżāēīūãõñăğșț
   */
class UsernameValidator
  implements
    \Ferrox\System\IValidator
{
  const LATIN_NUM_PATTERN = '0-9';
  const LATIN_ALPHA_PATTERN = 'a-z\xE6\xF0\x{0142}\xF8\x{0153}\xDF\xFE\x{0111}\x{0127}\x{0131}';
  const LETTER_ASH        = '\xE6';
  const LETTER_ETH        = '\xF0';
  const LETTER_SLASH_L    = '\x{0142}';
  const LETTER_SLASH_O    = '\xF8';
  const LETTER_SHARP_S    = '\xDF';
  const LETTER_OETHEL     = '\x{0153}';
  const LETTER_THORN      = '\xFE';
  const LETTER_D_BAR      = '\x{0111}';
  const LETTER_H_BAR      = '\x{0127}';
  const LETTER_DOTLESS_I  = '\x{0131}';
  const ACCENT_GRAVE      = '\x{0300}';
  const ACCENT_ACUTE      = '\x{0301}';
  const ACCENT_CIRCUMFLEX = '\x{0302}';
  const ACCENT_TILDE      = '\x{0303}';
  const ACCENT_MACRON     = '\x{0304}';
  const ACCENT_BREVE      = '\x{0306}';
  const ACCENT_OVERDOT    = '\x{0307}';
  const ACCENT_UMLAUT     = '\x{0308}';
  const ACCENT_RING       = '\x{030A}';
  const ACCENT_DOUBLE_ACUTE  = '\x{030B}';
  const ACCENT_HACEK      = '\x{030C}';
  const ACCENT_OGONEK     = '\x{0328}';
  const ACCENT_COMMA      = '\x{0326}';
  const ACCENT_CEDILLA    = '\x{0327}';
  const GREEK_ALPHA       = '\x{03B1}';
  const GREEK_OMEGA       = '\x{03C9}';
  const GREEK_EPSILON     = '\x{03B5}';
  const GREEK_ETA         = '\x{03B7}';
  const GREEK_IOTA        = '\x{03B9}';
  const GREEK_UPSILON     = '\x{03C5}';
  const GREEK_OMICRON     = '\x{03BF}';
  protected $_options = array();
  protected $_greekSupport = FALSE;
  protected $_cyrillicSupport = FALSE;
  protected $_japaneseSupport = FALSE;
  protected $_koreanSupport = FALSE;
  protected $_startWithAlphanum = TRUE;
  protected $_minLength = 4;
  protected $_maxLength = 31;
  protected $_regexCache = NULL;

  public function __construct ($options = array()) {

  }

  public function getMinLength () {
    return $this->_minLength;
  }
  public function getMaxLength () {
    return $this->_maxLength;
  }

  protected function startWithAlphanum () {
    return $this->_startWithAlphanum;
  }


  protected function supportGreek() {
    return $this->_greekSupport;
  }


  protected function supportCyrillic() {
    return $this->_cyrillicSupport;
  }

  protected function supportKorean () {
    return $this->_koreanSupport;
  }

  protected function supportJapanese () {
    return $this->_japaneseSupport;
  }

  /*
   * @param unknown_type $value
   */
  public function getRegex () {
    // Language Coverage
    if ($this->_regex !== NULL) {
      return $this->_regexCache;
    }

    $regex  = '^';
    if ($this->startWithAlphanum()) {
      $regex .= '[' . self::LATIN_ALPHA_PATTERN . self::LATIN_NUM_PATTERN . ']';
    }
    $regex .= '(?:';
    $regex .= '(?:[' . self::LATIN_ALPHA_PATTERN . self::LATIN_NUM_PATTERN . '])';
    $regex .= '|(?:[aeiou][' . self::ACCENT_GRAVE . '])';
    $regex .= '|(?:[aceilnorsuyz][' . self::ACCENT_ACUTE . '])';
    $regex .= '|(?:[aeiou][' . self::ACCENT_CIRCUMFLEX . '])';
    $regex .= '|(?:[aon][' . self::ACCENT_TILDE. '])';
    $regex .= '|(?:[aeiu][' . self::ACCENT_MACRON . '])';
    $regex .= '|(?:[ag][' . self::ACCENT_BREVE . '])';
    $regex .= '|(?:[cegz][' . self::ACCENT_OVERDOT . '])';
    $regex .= '|(?:[aeiouy][' . self::ACCENT_UMLAUT . '])';
    $regex .= '|(?:[au][' . self::ACCENT_RING . '])';
    $regex .= '|(?:[ou][' . self::ACCENT_DOUBLE_ACUTE . '])';
    $regex .= '|(?:[cdelnsrtz][' . self::ACCENT_HACEK . '])';
    $regex .= '|(?:[aeiu][' . self::ACCENT_OGONEK . '])';
    $regex .= '|(?:[st][' . self::ACCENT_COMMA . '])';
    $regex .= '|(?:[cgklnrs][' . self::ACCENT_CEDILLA . '])';

    if ($this->supportGreek()) {
      $regex .= '|(?:[' . self::GREEK_ALPHA . '-' . self::GREEK_OMEGA . '])';
      $regex .= '|(?:[' . self::GREEK_ALPHA . self::GREEK_EPSILON . self::GREEK_ETA
                        . self::GREEK_IOTA . self::GREEK_OMICRON . self::GREEK_UPSILON
                        . self::GREEK_OMEGA
                        . '][' . self::ACCENT_ACUTE . '])';
    }

    if ($this->supportCyrillic()) {
      $regex .= '|(?:[\x{0430}-\x{0438}\x{043A}-\x{044F}\x{0452}\x{0454}\x{0456}\x{0458}\x{0459}-\x{045B}\x{045F}\x{0491}])';
      $regex .= '|(?:[\x{0435}\x{0456}][' . self::ACCENT_UMLAUT . '])';
      $regex .= '|(?:[\x{0436}\x{0438}\x{0443}][' . self::ACCENT_BREVE . '])';
      $regex .= '|(?:[\x{0433}\x{043A}][' . self::ACCENT_ACUTE . '])';
      $regex .= '|(?:[\x{0435}\x{0438}][' . self::ACCENT_GRAVE . '])';
    }

    if ($this->supportKorean()) {
      // Variant: Modern
      $regex .= '|(?:[\x{1100}-\x{1113}\x{1161}-\x{1175}\x{11A8}-\x{11C2}])';
      // Variant: Historical
      // $regex .= '|(?:[\x{1100}-\x{11FF}\x{A960}-\x{A97C}\x{D7B0}-\x{D7FB}])';
    }

    if ($this->supportJapanese()) {

    }

    $regex .= ')';

    $regex .= '{' . $this->getMinLength() . ',' . $this->getMaxLength() . '}';
    $regex .= '$/u';

    $this->_regexCache = $regex;
    return $regex;
  }

}
