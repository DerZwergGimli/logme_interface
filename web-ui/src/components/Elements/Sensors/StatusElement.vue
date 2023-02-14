<template>
  <div v-if="props.status === E_SensorStatus.DEFAULT">
    <Button
      data-tooltip-target="tooltip-default"
      color="alternative"
      class="w-14"
    >
      <default-icon />
    </Button>
  </div>
  <div v-else-if="props.status === E_SensorStatus.IDLE">
    <Button color="alternative" class="w-14">
      <idle-icon />
    </Button>
  </div>
  <div v-else-if="props.status === E_SensorStatus.ERROR">
    <Button color="red" class="w-14 animate-pulse animate-duration-1s">
      <error-icon />
    </Button>
  </div>
  <div v-else-if="props.status === E_SensorStatus.READING">
    <Button color="yellow" class="w-14 animate-pulse animate-duration-2s">
      <reading-icon />
    </Button>
  </div>
  <div v-else>Undefined</div>
</template>

<script setup lang="ts">
import { PropType } from 'vue';
import { E_SensorStatus } from '../../../stores/interfaces/I_Sensor';
import { Button } from 'flowbite-vue';
import IdleIcon from '../../icons/IdleIcon.vue';
import DefaultIcon from '../../icons/DefaultIcon.vue';
import ErrorIcon from '../../icons/ErrorIcon.vue';
import ReadingIcon from '../../icons/ReadingIcon.vue';

const props = defineProps({
  status: {
    type: [] as PropType<E_SensorStatus>,
    default: E_SensorStatus.DEFAULT,
  },
});
</script>
