<template>
  <div>
    <div class="space-y-10">
      <div>
        <h3 class="text-center dark:text-white underline pb-2">WiFi Config</h3>
        <simple-table
          :header="wifi_config_table_header"
          :body="wifi_config_table_content"
        />
      </div>

      <div>
        <div class="relative">
          <h3
            class="w-full justify-center text-center dark:text-white underline pb-2"
          >
            Available Networks
          </h3>
          <div class="absolute bottom-1 right-1">
            <Button
              @click="
                () => {
                  createToast('Updating Networks', TOAST_INFO);
                  fetch_endpoint_ap();
                }
              "
              color="default"
              ><reload-icon
            /></Button>
          </div>
        </div>
        <simple-table
          :header="networks_table_header"
          :body="networks_table_content"
          :button_text="'Connect'"
          @button_clicked="
            value => {
              action_wifi_modal(value);
            }
          "
        />
      </div>
      {{ selected_network_ssid }}
    </div>
  </div>
  <div v-if="modalShown">
    <wifi-credentials-modal
      :is-shown="modalShown"
      :ssid="selected_network_ssid"
      @closeModal="modalShown = false"
    />
  </div>
</template>

<script setup lang="ts">
import { onMounted, ref } from 'vue';
import SimpleTable from '../../tables/SimpleTable.vue';
import { I_AccessPoint } from '../../../stores/interfaces/t_accesspoint';
import { Button } from 'flowbite-vue';
import ReloadIcon from '../../icons/ReloadIcon.vue';
import WifiCredentialsModal from '../../modals/WifiCredentialsModal.vue';
import { createToast } from 'mosha-vue-toastify';
import { TOAST_INFO } from '../../../scripts/toast_config';

const modalShown = ref(false);
const selected_network_ssid = ref('network_ssid');

const wifi_config_table_header = ref(['WiFi', 'Value']);
const wifi_config_table_content = ref([
  ['SSID', '---'],
  ['RSSI', '---'],
  ['IP', '---'],
  ['Netmask', '---'],
  ['Gateway', '---'],
]);

const networks_table_header = ref(['SSID', 'RSSI', 'AUTH', 'CHANNEL']);
const networks_table_content = ref([]);

onMounted(async () => {
  fetch_endpoint_wifi();
  fetch_endpoint_ap();
});

function fetch_endpoint_wifi() {
  // FETCH WIFI ENDPOINT
  fetch(APP_API_URL + '/wifi')
    .then(resp => resp.json())
    .then(json => {
      console.info(json);
      wifi_config_table_content.value[0][1] = json.status.ssid;
      wifi_config_table_content.value[1][1] = json.status.rssi;
      wifi_config_table_content.value[2][1] = json.status.ip;
      wifi_config_table_content.value[3][1] = json.status.netmask;
      wifi_config_table_content.value[4][1] = json.status.gateway;
    })
    .catch(err => console.error(err));
}

function fetch_endpoint_ap() {
  // FETCH AP ENDPOINT
  fetch(APP_API_URL + '/ap')
    .then(resp => resp.json())
    .then(json => {
      let ap_list = json as Array<I_AccessPoint>;
      networks_table_content.value = [];
      ap_list.forEach(ap => {
        networks_table_content.value.push([
          ap.ssid,
          ap.rssi,
          ap.auth,
          ap.chan,
        ] as never);
      });
    })
    .catch(err => console.error(err));
}

function action_wifi_modal(network_ssid: any) {
  selected_network_ssid.value = network_ssid[0];
  modalShown.value = true;
}
</script>
