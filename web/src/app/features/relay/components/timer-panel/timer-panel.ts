import { Component, input } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { ActionSwitch } from '../../../../shared/components/action-switch/action-switch';
import { Field } from '../../../../shared/components/field/field';
import { PrimaryButton } from '../../../../shared/components/primary-button/primary-button';
import { RelayVm } from '../../relay.vm';

@Component({
  selector: 'app-timer-panel',
  standalone: true,
  imports: [FormsModule, ActionSwitch, Field, PrimaryButton],
  templateUrl: './timer-panel.html',
  styleUrl: './timer-panel.scss',
})
export class TimerPanel {
  readonly vm = input.required<RelayVm>();
}
