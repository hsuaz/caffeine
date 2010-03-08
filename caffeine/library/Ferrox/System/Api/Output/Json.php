<?php
namespace Ferrox\System\Api\Output;

class Json
  implements IOutputFormatter
{
  public function send($data = array()) {
    if (!\is_array($data)) {
      throw Exception('Unknown data to format.');
    }

    header('Content-type: application/javascript');
    echo json_encode($data, JSON_FORCE_OBJECT);
  }
}