<template>
  <div>
    <div class="flex flex-col m-3 space-y-5">
      <SimpleTable
        :header="['REST GET-Requests']"
        :body="[['/system'], ['/wifi'], ['/sensors']]"
        :button_text="'GET'"
        @button_clicked="handle_get_button"
      />
      <SimpleTable
        :header="['REST POST-Requests']"
        :body="[['/wifiConfig']]"
        :button_text="'POST'"
      />
      <SimpleTable
        :header="['Websocket']"
        :body="[['/websocket']]"
        :button_text="'WS'"
      />
    </div>
  </div>
</template>
<script setup lang="ts">
import SimpleTable from '../tables/SimpleTable.vue';

function handle_get_button(event: [string]) {
  window.open(event[0], '_blank');
}
</script>
