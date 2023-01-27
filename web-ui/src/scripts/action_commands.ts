import { createToast } from 'mosha-vue-toastify';
import {
  TOAST_ERROR,
  TOAST_INFO,
  TOAST_SUCCESS,
  TOAST_WARNING,
} from './toast_config';

interface I_Response_Message {
  code: Number;
  status: String;
  message: String;
}

export function action_post(endpoint: String) {
  createToast('Sending command: ' + endpoint, TOAST_INFO);
  fetch(APP_API_URL + endpoint, {
    method: 'POST',
  })
    .then(response => response.json())
    .then(response => {
      make_toast(response);
    })
    .catch(err => {
      console.error(err);
      createToast('Error sending: POST ' + endpoint, TOAST_ERROR);
    });
}

function make_toast(message: I_Response_Message) {
  switch (message.code) {
    case 0:
      createToast(message.message, TOAST_SUCCESS);
      break;
    case 1:
      createToast(message.message, TOAST_ERROR);
      break;
    case 2:
      createToast(message.message, TOAST_ERROR);
      break;
    default:
      createToast(message.message, TOAST_WARNING);
      break;
  }
}
