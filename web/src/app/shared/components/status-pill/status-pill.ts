import { Component, computed, input } from '@angular/core';
import { RelayState } from '../../../core/models/relay.models';

@Component({
  selector: 'app-status-pill',
  standalone: true,
  templateUrl: './status-pill.html',
  styleUrl: './status-pill.scss',
})
export class StatusPill {
  readonly label = input.required<string>();
  readonly state = input<RelayState | 'normal'>('normal');

  protected readonly tone = computed(() => {
    const state = this.state();
    if (state === 'on') return 'is-on';
    if (state === 'scheduled') return 'is-scheduled';
    if (state === 'off') return 'is-off';
    return 'is-neutral';
  });
}
