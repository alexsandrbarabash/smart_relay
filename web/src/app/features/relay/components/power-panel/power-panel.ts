import { Component, input } from '@angular/core';
import { ActionSwitch } from '../../../../shared/components/action-switch/action-switch';
import { PrimaryButton } from '../../../../shared/components/primary-button/primary-button';
import { RelayVm } from '../../relay.vm';

@Component({
  selector: 'app-power-panel',
  standalone: true,
  imports: [ActionSwitch, PrimaryButton],
  templateUrl: './power-panel.html',
  styleUrl: './power-panel.scss',
})
export class PowerPanel {
  readonly vm = input.required<RelayVm>();
}
