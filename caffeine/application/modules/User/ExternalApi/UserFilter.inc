<?php

declare(encoding='UTF-8');
namespace Ferrox\Caffeine\User\ExternalApi;

class UserFilter {
  public function filter ($value) {
    if (array_key_exists('user', $value)) {
      unset($value['user']['passwordHash']);
      unset($value['user']['passwordSalt']);
    }
    return $value;
  }
}