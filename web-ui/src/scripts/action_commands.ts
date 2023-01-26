import { createToast } from 'mosha-vue-toastify';
import { TOAST_ERROR, TOAST_INFO, TOAST_SUCCESS } from './toast_config';

export function action_post(endpoint: String) {
  createToast('Sending command: ' + endpoint, TOAST_INFO);
  fetch(APP_API_URL + endpoint, {
    method: 'POST',
  })
    .then(response => response.text())
    .then(response => {
      console.log(response);
      createToast(response, TOAST_SUCCESS);
    })
    .catch(err => {
      console.error(err);
      createToast('Error sending: POST ' + endpoint, TOAST_ERROR);
    });
}
