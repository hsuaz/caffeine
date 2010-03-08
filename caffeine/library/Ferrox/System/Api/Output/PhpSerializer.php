<?php
namespace Ferrox\System\Api\Output;

class PhpSerializer
  implements IOutputFormatter
{
  public function send($data = array()) {
    if (!\is_array($data)) {
      throw Exception('Unknown data to format.');
    }

    header('Content-type: text/plain; charset=UTF-8');
    echo serialize($data);
  }
}